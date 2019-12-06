/*
  ==============================================================================

    pMixGraphEditorParts.cpp
    based on JUCE demo host
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixGraphEditorParts.h"
#include "pMixGraphEditorActions.h"
#include "pMixConstants.h"

#pragma mark -
#pragma mark PinComponent

PinComponent::PinComponent (PMixAudioEngine& audioEngine, NodeID nodeID_, const int index_, const bool isInput_)
: nodeID (nodeID_)
, index (index_)
, isInput (isInput_)
, mouseOver(false)
, busIdx(0)
, audioEngine(audioEngine)
{
  if (const AudioProcessorGraph::Node::Ptr node = audioEngine.getDoc().getNodeForId (nodeID_))
  {
    String tip;
    
    if (index_ == PMixDocument::midiChannelNumber)
    {
      tip = isInput ? "MIDI Input" : "MIDI Output";
    }
    else
    {
      const AudioProcessor& processor = *node->getProcessor();
      
      int channel;
      channel = processor.getOffsetInBusBufferForAbsoluteChannelIndex (isInput, index, busIdx);
      
      if (const AudioProcessor::Bus* bus = processor.getBus (isInput, busIdx))
        tip = bus->getName() + String (": ")
        + AudioChannelSet::getAbbreviatedChannelTypeName (bus->getCurrentLayout().getTypeOfChannel (channel));
      else
        tip = (isInput ? "Main Input: "
               : "Main Output: ") + String (index + 1);
    }
    
    setTooltip (tip);
  }
  
  setSize (16, 16);
}

void PinComponent::paint (Graphics& g)
{
//  const float w = (float) getWidth();
//  const float h = (float) getHeight();
  
  if (mouseOver)
  {
    Path p;
    p.addRoundedRectangle(0.f, isInput ? 11.f : 0.0f, 16.f, 6.f, 1.f, 1.f, isInput, isInput, !isInput, !isInput);
    g.setColour (Colours::lightgrey);
    g.fillPath(p);
  }
  
  Path p;
  p.addRoundedRectangle(2.f, isInput ? 13.f : 0.0f, 12.f, 3.f, 1.f, 1.f, isInput, isInput, !isInput, !isInput);
  g.setColour (index == PMixDocument::midiChannelNumber ? Colours::grey : Colours::black);
  g.fillPath(p);
}

void PinComponent::mouseDown (const MouseEvent& e)
{
  getGraphEditor()->beginConnectorDrag (isInput ? NodeID(0) : nodeID, index, isInput ? nodeID : NodeID(0), index, e);
}

void PinComponent::mouseDrag (const MouseEvent& e)
{
  getGraphEditor()->dragConnector (e);
}

void PinComponent::mouseUp (const MouseEvent& e)
{
  getGraphEditor()->endDraggingConnector (e);
}

void PinComponent::mouseEnter (const MouseEvent& e)
{
  mouseOver = true;
  repaint();
}

void PinComponent::mouseExit (const MouseEvent& e)
{
  mouseOver = false;
  repaint();
}

GraphEditor* PinComponent::getGraphEditor() const noexcept
{
  return findParentComponentOfClass<GraphEditor>();
}

#pragma mark -
#pragma mark NodeComponent

NodeComponent::NodeComponent (PMixAudioEngine& audioEngine, NodeID nodeID_)
: audioEngine (audioEngine)
, nodeID (nodeID_)
, numInputs (0)
, numOutputs (0)
, pinSize (16)
, font (13.0f, Font::bold)
, numIns (0)
, numOuts (0)
, moving(false)
, editor(nullptr)
, nodeName(nullptr)
, highlight(false)
, bbl(5000)
{
  setSize (100, 50);
}

NodeComponent::~NodeComponent()
{
  deleteAllChildren();
}

void NodeComponent::removeEditor()
{
  if (editor != nullptr)
    delete editor;
  
  editor = nullptr;
}

void NodeComponent::mouseDown (const MouseEvent& e)
{
  originalPos = localPointToGlobal (Point<int>());
  
  toFront (true);
  
  if (e.mods.isPopupMenu())
  {
    PopupMenu m;
    m.addItem (1, TRANS("Delete this node"));
    m.addItem (2, TRANS("Disconnect all pins"));
    
    if (AudioProcessorGraph::Node::Ptr f = audioEngine.getDoc().getNodeForId (nodeID))
    {
      AudioProcessor* const processor = f->getProcessor();
      jassert (processor != nullptr);
      
      if(!InternalPluginFormat::isInternalFormat(processor->getName()))
      {
        bool hasParams = (processor->getNumParameters() > 0);
        
        m.addItem (3, TRANS("Add a pMix Preset"), hasParams);
        m.addItem (4, TRANS("Set pMix Colour"), hasParams);
        m.addItem (5, TRANS("Interpolate all Parameters"), hasParams);
        m.addItem (6, TRANS("Clear all Parameters"), hasParams);
        m.addSeparator();
        
        int uiStatus = f->properties["uiStatus"];
        
        PopupMenu ui;
        ui.addItem (7, TRANS("Disabled"), true, uiStatus == kUIStatusNone);
        ui.addItem (8, TRANS("Embedded"), true, uiStatus == kUIStatusEmbed);
//        ui.addItem (9, TRANS("Floating"), true, uiStatus == kUIStatusFloating);

        m.addSubMenu (TRANS("Set UI Mode"), ui);
      }
    }
    
    const int r = m.show();
    
    if (r == 1)
    {
      if (AudioProcessorGraph::Node::Ptr f = audioEngine.getDoc().getNodeForId (nodeID))
      {
        AudioPluginInstance* const instance = dynamic_cast<AudioPluginInstance*>(f->getProcessor());
      
        if (instance)
        {
          removeEditor();
          
          audioEngine.getDoc().beginTransaction();
          audioEngine.getDoc().perform(new RemoveNodeAction(audioEngine, *getGraphEditor(), nodeID), TRANS("remove node"));
          
          getGraphEditor()->clearSelection();
        }
      }
      return;
    }
    else if (r == 2)
    {
      audioEngine.getDoc().disconnectNode (nodeID);
    }
    else if (r == 3)
    {
      Random rand;
      audioEngine.getDoc().addPreset(nodeID, rand.nextFloat(), rand.nextFloat());
    }
    else if (r == 4)
    {
      ColourSelector* colourSelector = new ColourSelector(ColourSelector::showSliders|ColourSelector::showColourAtTop|ColourSelector::showColourspace);
      colourSelector->setName ("background");
      colourSelector->setCurrentColour (audioEngine.getDoc().getNodeColour(nodeID));
      colourSelector->addChangeListener (this);
      colourSelector->setColour (ColourSelector::backgroundColourId, Colours::lightgrey);
      colourSelector->setSize (300, 400);
      
      CallOutBox::launchAsynchronously (colourSelector, getScreenBounds(), nullptr);
    }
    else if (r == 5 || r == 6)
    {
      if (AudioProcessorGraph::Node::Ptr f = audioEngine.getDoc().getNodeForId (nodeID))

      {
        for (int p=0; p < f->getProcessor()->getNumParameters(); p++)
        {
          audioEngine.getDoc().setParameterToInterpolate(nodeID, p, r==5);
        }
        
        repaint();
      }
    }
    else if (r == 7)
    {
      audioEngine.getDoc().setNodeUIStatus(nodeID, kUIStatusNone);
      removeEditor();
      PluginWindow::closeCurrentlyOpenWindowsFor(nodeID);
      update();
    }
    else if (r == 8)
    {
      audioEngine.getDoc().setNodeUIStatus(nodeID, kUIStatusEmbed);
      PluginWindow::closeCurrentlyOpenWindowsFor(nodeID);
      update();
    }
//    else if (r == 9)
//    {
//      if (AudioProcessorGraph::Node::Ptr f = audioEngine.getDoc().getNodeForId (nodeID))
//      {
//        AudioProcessor* const processor = f->getProcessor();
//        jassert (processor != nullptr);
//        
//        String name = processor->getName();
//        
//        if (r > 0)
//        {
//          removeEditor();
//
//          PluginWindow::WindowFormatType type = processor->hasEditor() ? PluginWindow::Normal : PluginWindow::Generic;
//          
//          if (PluginWindow* const w = PluginWindow::getWindowFor (f, type))
//            w->toFront (true);
//          
//          audioEngine.getDoc().setNodeUIStatus(nodeID, kUIStatusFloating);
//          update();
//        };
//      }
//    }
  }
  else
  {
    moving = true;
    getGraphEditor()->getLassoSelection().selectOnly(this);
    audioEngine.getDoc().getNodePosition(nodeID, startPos.x, startPos.y);
  }
}

void NodeComponent::mouseDrag (const MouseEvent& e)
{
  if (! e.mods.isPopupMenu())
  {
    Point<int> pos (originalPos + Point<int> (e.getDistanceFromDragStartX(), e.getDistanceFromDragStartY()));
    
    if (getParentComponent() != nullptr)
      pos = getParentComponent()->getLocalPoint (nullptr, pos);
    
    endPos.x = (pos.getX() + getWidth() / 2) / (double) getParentWidth();
    endPos.y = (pos.getY() + getHeight() / 2) / (double) getParentHeight();
    
    audioEngine.getDoc().setNodePosition (nodeID, endPos.x, endPos.y);
    
    getGraphEditor()->updateComponents();
  }
}

void NodeComponent::mouseUp (const MouseEvent& e)
{
  if (e.mouseWasClicked() && e.getNumberOfClicks() == 2)
  {
    if (const AudioProcessorGraph::Node::Ptr f = audioEngine.getDoc().getNodeForId (nodeID))
    {
      AudioProcessor* const processor = f->getProcessor();
      if(!InternalPluginFormat::isInternalFormat(processor->getName()))
      {
        if (PluginWindow* const w = PluginWindow::getWindowFor (f, PluginWindow::Generic/*Normal*/))
          w->toFront (true);
      }
    }
  }
  else if (! e.mouseWasClicked())
  {
    audioEngine.getDoc().setChangedFlag (true);
    
    if (moving)
    {
      moving = false;
      audioEngine.getDoc().beginTransaction();
      audioEngine.getDoc().perform(new MoveNodeAction(audioEngine, *getGraphEditor(), nodeID, startPos, endPos), "move node");
    }
  }
}

void NodeComponent::mouseDoubleClick (const MouseEvent&)
{
  if (AudioProcessorGraph::Node::Ptr f = audioEngine.getDoc().getNodeForId (nodeID))
  {
    AudioProcessor* const processor = f->getProcessor();
    jassert (processor != nullptr);
    
    String name = processor->getName();
    
    PluginWindow::WindowFormatType type = processor->hasEditor() ? PluginWindow::Normal : PluginWindow::Generic;
    
    if (PluginWindow* const w = PluginWindow::getWindowFor (f, type))
      w->toFront (true);
  }
}

bool NodeComponent::hitTest (int x, int y)
{
  for (int i = getNumChildComponents(); --i >= 0;)
    if (getChildComponent(i)->getBounds().contains (x, y))
      return true;
  
  return x >= 3 && x < getWidth() - 6 && y >= pinSize && y < getHeight() - pinSize;
}

void NodeComponent::paint (Graphics& g)
{
  g.setColour (Colours::lightgrey);
  
  if (highlight)
    g.setColour (Colours::white);

  const int x = 4;
  const int y = pinSize;
  const int w = getWidth() - x * 2;
  const int h = getHeight() - pinSize * 2;
  
  g.fillRect(x, y, w, h);

  g.setColour (Colours::black);
  
  if (getGraphEditor()->getLassoSelection().isSelected(this))
    g.setColour (Colours::black);
  else
    g.setColour (Colours::grey);
  
  g.drawRoundedRectangle(x, y, w, h, 2, 1);
}

void NodeComponent::resized()
{
  for (int i = 0; i < getNumChildComponents(); ++i)
  {
    if (PinComponent* const pc = dynamic_cast <PinComponent*> (getChildComponent(i)))
    {
      const int total = pc->isInput ? numIns : numOuts;
      const int index = pc->index == PMixDocument::midiChannelNumber ? (total - 1) : pc->index;
      
      pc->setBounds(PINS_LEFT_OFFSET + (index * OFFSET_BETWEEN_PINS), pc->isInput ? 0 : (getHeight() - pinSize), pinSize, pinSize);
    }
  }
  
  if (nodeName != nullptr)
    nodeName->setBounds(0, pinSize, getWidth(), 20);
  
  if (editor != nullptr)
    editor->setBounds(10, pinSize + nodeName->getHeight(), getWidth()-20, getHeight() - pinSize - pinSize - 10 - nodeName->getHeight());
  
}

void NodeComponent::getPinPos (const int index, const bool isInput, float& x, float& y)
{
  for (int i = 0; i < getNumChildComponents(); ++i)
  {
    if (PinComponent* const pc = dynamic_cast <PinComponent*> (getChildComponent(i)))
    {
      if (pc->index == index && isInput == pc->isInput)
      {
        x = getX() + pc->getX() + pc->getWidth() * 0.5f;
        y = getY() + pc->getY() + pc->getHeight() * 0.5f;
        break;
      }
    }
  }
}

void NodeComponent::update()
{
  const AudioProcessorGraph::Node::Ptr f (audioEngine.getDoc().getNodeForId (nodeID));
  
  if (f == nullptr)
  {
    delete this;
    return;
  }

  numIns = f->getProcessor()->getTotalNumInputChannels();
  if (f->getProcessor()->acceptsMidi())
    ++numIns;
  
  numOuts = f->getProcessor()->getTotalNumOutputChannels();
  if (f->getProcessor()->producesMidi())
    ++numOuts;
  
  FaustAudioPluginInstance* faustProc = dynamic_cast<FaustAudioPluginInstance*>(f->getProcessor());
  
  if (faustProc)
    highlight = faustProc->getHighlight();

  int w = 10;
  int h = 50;

  // Update width based on number of I/O
  w = jmax (w, PINS_LEFT_OFFSET + ((jmax (numIns, numOuts) + 1) * OFFSET_BETWEEN_PINS));
  
  String name = f->getProcessor()->getName();
  
  setName (name);

  if (numIns != numInputs || numOuts != numOutputs || editor == nullptr)
  {
    numInputs = numIns;
    numOutputs = numOuts;
    
    deleteAllChildren();
    
    addAndMakeVisible(nodeName = new Label(name, name));
    nodeName->setJustificationType(Justification::centred);
    nodeName->setInterceptsMouseClicks(false, false);
    nodeName->setFont(font);
    
    w = jmax (w, nodeName->getFont().getStringWidth(name) + 15);
  
    if(!InternalPluginFormat::isInternalFormat(name) && f->getProcessor()->getNumParameters() > 0)
    {
      int uiStatus = f->properties["uiStatus"];
      
      if(uiStatus == kUIStatusEmbed)
      {
        addAndMakeVisible(editor = new PMixGenericAudioProcessorEditor (audioEngine, f->getProcessor(), f->nodeID));
        w = jmax (w, editor->getWidth() + 20 );
        
        if (editor->getContentHeight() > 300) {
          editor->setSize(editor->getWidth(), 100);
        }
        
        h += jmin (320, editor->getContentHeight() + 20);
      }
    }
    
    setSize (w, h);

    int i;
    for (i = 0; i < f->getProcessor()->getTotalNumInputChannels(); ++i)
      addAndMakeVisible (new PinComponent (audioEngine, nodeID, i, true));
    
    if (f->getProcessor()->acceptsMidi())
      addAndMakeVisible (new PinComponent (audioEngine, nodeID, PMixDocument::midiChannelNumber, true));
    
    for (i = 0; i < f->getProcessor()->getTotalNumOutputChannels(); ++i)
      addAndMakeVisible (new PinComponent (audioEngine, nodeID, i, false));
    
    if (f->getProcessor()->producesMidi())
      addAndMakeVisible (new PinComponent (audioEngine, nodeID, PMixDocument::midiChannelNumber, false));
    
    resized();
  }
  
  {
    double x, y;
    audioEngine.getDoc().getNodePosition (nodeID, x, y);
    setCentreRelative ((float) x, (float) y);
  }
    
  if(faustProc != nullptr) {
    if(faustProc->getCompilerMessage() != String())
      bubbleMessage(faustProc->getCompilerMessage());
  }
}

GraphEditor* NodeComponent::getGraphEditor() const noexcept
{
  return findParentComponentOfClass<GraphEditor>();
}

void NodeComponent::changeListenerCallback (ChangeBroadcaster* source)
{
  if (ColourSelector* cs = dynamic_cast <ColourSelector*> (source))
  {    
    audioEngine.getDoc().setNodeColour(nodeID, cs->getCurrentColour());
    
    if (editor != nullptr)
      editor->repaint();
  }
}

void NodeComponent::bubbleMessage(String msg)
{
  AttributedString text (msg);
  text.setJustification (Justification::centred);
  bbl.setAlwaysOnTop (true);
  bbl.addToDesktop (0);
  
  bbl.showAt(this, text, 5000., true, false);
}


#pragma mark -
#pragma mark ConnectorComponent

ConnectorComponent::ConnectorComponent (PMixAudioEngine& audioEngine)
: sourceNodeId (0)
, destNodeId (0)
, sourceNodeChannel (0)
, destNodeChannel (0)
, audioEngine (audioEngine)
, lastInputX (0)
, lastInputY (0)
, lastOutputX (0)
, lastOutputY (0)
{
  //setAlwaysOnTop (true);
}

void ConnectorComponent::setInput (NodeID sourceNodeId_, const int sourceNodeChannel_)
{
  if (sourceNodeId != sourceNodeId_ || sourceNodeChannel != sourceNodeChannel_)
  {
    sourceNodeId = sourceNodeId_;
    sourceNodeChannel = sourceNodeChannel_;
    update();
  }
}

void ConnectorComponent::setOutput (NodeID destNodeId_, const int destNodeChannel_)
{
  if (destNodeId != destNodeId_ || destNodeChannel != destNodeChannel_)
  {
    destNodeId = destNodeId_;
    destNodeChannel = destNodeChannel_;
    update();
  }
}

void ConnectorComponent::dragStart (int x, int y)
{
  lastInputX = (float) x;
  lastInputY = (float) y;
  resizeToFit();
}

void ConnectorComponent::dragEnd (int x, int y)
{
  lastOutputX = (float) x;
  lastOutputY = (float) y;
  resizeToFit();
}

void ConnectorComponent::update()
{
  float x1, y1, x2, y2;
  getPoints (x1, y1, x2, y2);
  
  if (lastInputX != x1
      || lastInputY != y1
      || lastOutputX != x2
      || lastOutputY != y2)
  {
    resizeToFit();
  }
}

void ConnectorComponent::resizeToFit()
{
  float x1, y1, x2, y2;
  getPoints (x1, y1, x2, y2);
  
  const Rectangle<int> newBounds ((int) jmin (x1, x2) - 4,
                                  (int) jmin (y1, y2) - 4,
                                  (int) fabsf (x1 - x2) + 8,
                                  (int) fabsf (y1 - y2) + 8);
  
  if (newBounds != getBounds())
    setBounds (newBounds);
  else
    resized();
  
  repaint();
}

void ConnectorComponent::getPoints (float& x1, float& y1, float& x2, float& y2) const
{
  x1 = lastInputX;
  y1 = lastInputY;
  x2 = lastOutputX;
  y2 = lastOutputY;
  
  if (GraphEditor* const hostPanel = getGraphEditor())
  {
    if (NodeComponent* srcNodeComp = hostPanel->getComponentForNode (sourceNodeId))
      srcNodeComp->getPinPos (sourceNodeChannel, false, x1, y1);
    
    if (NodeComponent* dstNodeComp = hostPanel->getComponentForNode (destNodeId))
      dstNodeComp->getPinPos (destNodeChannel, true, x2, y2);
  }
}

void ConnectorComponent::paint (Graphics& g)
{
  if (sourceNodeChannel == PMixDocument::midiChannelNumber
      || destNodeChannel == PMixDocument::midiChannelNumber)
  {
    g.setColour (Colours::grey);
  }
  else
  {
    g.setColour (Colours::black);
  }
  
  g.fillPath (linePath);
}

bool ConnectorComponent::hitTest (int x, int y)
{
  if (hitPath.contains ((float) x, (float) y))
  {
    double distanceFromStart, distanceFromEnd;
    getDistancesFromEnds (x, y, distanceFromStart, distanceFromEnd);
    
    // avoid clicking the connector when over a pin
    return distanceFromStart > 7.0 && distanceFromEnd > 7.0;
  }
  
  return false;
}

void ConnectorComponent::mouseDown (const MouseEvent&)
{
  dragging = false;
}

void ConnectorComponent::mouseDrag (const MouseEvent& e)
{
  if ((! dragging) && ! e.mouseWasClicked())
  {
    dragging = true;
    
    audioEngine.getDoc().removeConnection (sourceNodeId, sourceNodeChannel, destNodeId, destNodeChannel);
    
    double distanceFromStart, distanceFromEnd;
    getDistancesFromEnds (e.x, e.y, distanceFromStart, distanceFromEnd);
    const bool isNearerSource = (distanceFromStart < distanceFromEnd);
    
    getGraphEditor()->beginConnectorDrag (isNearerSource ? NodeID(0) : sourceNodeId,
                                         sourceNodeChannel,
                                         isNearerSource ? destNodeId : NodeID(0),
                                         destNodeChannel,
                                         e);
  }
  else if (dragging)
  {
    getGraphEditor()->dragConnector (e);
  }
}

void ConnectorComponent::mouseUp (const MouseEvent& e)
{
  if (dragging)
    getGraphEditor()->endDraggingConnector (e);
}

void ConnectorComponent::resized()
{
  float x1, y1, x2, y2;
  getPoints (x1, y1, x2, y2);
  
  lastInputX = x1;
  lastInputY = y1;
  lastOutputX = x2;
  lastOutputY = y2;
  
  x1 -= getX();
  y1 -= getY();
  x2 -= getX();
  y2 -= getY();
  
  linePath.clear();
  linePath.startNewSubPath (x1, y1);
  //linePath.lineTo(x2, y2);
  linePath.cubicTo (x1, y1 + (y2 - y1) * 0.33f,
                    x2, y1 + (y2 - y1) * 0.66f,
                    x2, y2);
  
  PathStrokeType wideStroke (8.0f);
  wideStroke.createStrokedPath (hitPath, linePath);
  
  PathStrokeType stroke (1.5f);
  //stroke.createStrokedPath (linePath, linePath);
  float dashes[2] = { 4, 4 };
  stroke.createDashedStroke(linePath, linePath, dashes, 2);
  
  const float arrowW = 3.0f;
  const float arrowL = 2.0f;
  
  Path arrow;
  arrow.addTriangle (-arrowL, arrowW,
                     -arrowL, -arrowW,
                     arrowL, 0.0f);
  
  arrow.applyTransform (AffineTransform()
                        .rotated (float_Pi * 0.5f - (float) atan2 (x2 - x1, y2 - y1))
                        .translated ((x1 + x2) * 0.5f,
                                     (y1 + y2) * 0.5f));
  
  linePath.addPath (arrow);
  linePath.setUsingNonZeroWinding (true);
}

GraphEditor* ConnectorComponent::getGraphEditor() const noexcept
{
  return findParentComponentOfClass<GraphEditor>();
}

void ConnectorComponent::getDistancesFromEnds (int x, int y, double& distanceFromStart, double& distanceFromEnd) const
{
  float x1, y1, x2, y2;
  getPoints (x1, y1, x2, y2);
  
  distanceFromStart = juce_hypot (x - (x1 - getX()), y - (y1 - getY()));
  distanceFromEnd = juce_hypot (x - (x2 - getX()), y - (y2 - getY()));
}
