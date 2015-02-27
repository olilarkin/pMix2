/*
  ==============================================================================

    pMixGraphEditorParts.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixGraphEditorParts.h"
#include "pMixGraphEditorActions.h"

#pragma mark -
#pragma mark PinComponent

PinComponent::PinComponent (PMixAudioEngine& audioEngine, const uint32 filterID_, const int index_, const bool isInput_)
: filterID (filterID_),
index (index_),
isInput (isInput_),
audioEngine(audioEngine)
{
  if (const AudioProcessorGraph::Node::Ptr node = audioEngine.getDoc().getNodeForId (filterID_))
  {
    String tip;
    
    if (index_ == PMixDocument::midiChannelNumber)
    {
      tip = isInput ? "MIDI Input" : "MIDI Output";
    }
    else
    {
      if (isInput)
        tip = node->getProcessor()->getInputChannelName (index_);
      else
        tip = node->getProcessor()->getOutputChannelName (index_);
      
      if (tip.isEmpty())
        tip = (isInput ? "Input " : "Output ") + String (index_ + 1);
    }
    
    setTooltip (tip);
  }
  
  setSize (16, 16);
}

void PinComponent::paint (Graphics& g)
{
  const float w = (float) getWidth();
  const float h = (float) getHeight();
  
  Path p;
  p.addEllipse (w * 0.25f, h * 0.25f, w * 0.5f, h * 0.5f);
  
  p.addRectangle (w * 0.4f, isInput ? (0.5f * h) : 0.0f, w * 0.2f, h * 0.5f);
  
  g.setColour (index == PMixDocument::midiChannelNumber ? Colours::red : Colours::green);
  g.fillPath (p);
}

void PinComponent::mouseDown (const MouseEvent& e)
{
  getGraphPanel()->beginConnectorDrag (isInput ? 0 : filterID,
                                       index,
                                       isInput ? filterID : 0,
                                       index,
                                       e);
}

void PinComponent::mouseDrag (const MouseEvent& e)
{
  getGraphPanel()->dragConnector (e);
}

void PinComponent::mouseUp (const MouseEvent& e)
{
  getGraphPanel()->endDraggingConnector (e);
}

GraphEditor* PinComponent::getGraphPanel() const noexcept
{
  return findParentComponentOfClass<GraphEditor>();
}

#pragma mark -
#pragma mark FilterComponent

FilterComponent::FilterComponent (PMixAudioEngine& audioEngine, const uint32 filterID_)
: audioEngine (audioEngine)
, filterID (filterID_)
, numInputs (0)
, numOutputs (0)
, pinSize (16)
, font (13.0f, Font::bold)
, numIns (0)
, numOuts (0)
, moving(false)
, editor(nullptr)
, filterName(nullptr)
{
  setSize (100, 50);
}

FilterComponent::~FilterComponent()
{
  deleteAllChildren();
}

void FilterComponent::mouseDown (const MouseEvent& e)
{
  originalPos = localPointToGlobal (Point<int>());
  
  toFront (true);
  
  if (e.mods.isPopupMenu())
  {
    PopupMenu m;
    m.addItem (1, "Delete this filter");
    m.addItem (2, "Disconnect all pins");
    
    if (AudioProcessorGraph::Node::Ptr f = audioEngine.getDoc().getNodeForId (filterID))
    {
      AudioProcessor* const processor = f->getProcessor();
      jassert (processor != nullptr);
      
      String name = processor->getName();
      if(name != "Audio Input" && name != "Audio Output" && name != "Midi Input" && name != "Midi Output")
      {
        m.addSeparator();
        m.addItem (3, "Show plugin UI");
        m.addItem (4, "Show all parameters");
      }
    }
    
    const int r = m.show();
    
    if (r == 1)
    {
      if (AudioProcessorGraph::Node::Ptr f = audioEngine.getDoc().getNodeForId (filterID))
      {
        AudioPluginInstance* const instance = dynamic_cast<AudioPluginInstance*>(f->getProcessor());
        
        if (instance)
        {
          audioEngine.getDoc().beginTransaction();
          audioEngine.getDoc().perform(new RemoveFilterAction(audioEngine, filterID), TRANS("remove filter"));
        }
      }
      return;
    }
    else if (r == 2)
    {
      audioEngine.getDoc().disconnectFilter (filterID);
    }
    else
    {
      if (AudioProcessorGraph::Node::Ptr f = audioEngine.getDoc().getNodeForId (filterID))
      {
        AudioProcessor* const processor = f->getProcessor();
        jassert (processor != nullptr);
        
        String name = processor->getName();
        
        if (r > 0)
        {
          PluginWindow::WindowFormatType type = processor->hasEditor() ? PluginWindow::Normal
          : PluginWindow::Generic;
          
          switch (r)
          {
            case 4: type = PluginWindow::Parameters; break;
              
            default: break;
          };
          
          if (PluginWindow* const w = PluginWindow::getWindowFor (f, type))
            w->toFront (true);
        }
      }
    }
  }
  else
  {
    moving = true;
    getGraphPanel()->getLassoSelection().selectOnly(this);
    audioEngine.getDoc().getNodePosition(filterID, startPos.x, startPos.y);
  }
}

void FilterComponent::mouseDrag (const MouseEvent& e)
{
  if (! e.mods.isPopupMenu())
  {
    Point<int> pos (originalPos + Point<int> (e.getDistanceFromDragStartX(), e.getDistanceFromDragStartY()));
    
    if (getParentComponent() != nullptr)
      pos = getParentComponent()->getLocalPoint (nullptr, pos);
    
    endPos.x = (pos.getX() + getWidth() / 2) / (double) getParentWidth();
    endPos.y = (pos.getY() + getHeight() / 2) / (double) getParentHeight();
    
    audioEngine.getDoc().setNodePosition (filterID, endPos.x, endPos.y);
    
    getGraphPanel()->updateComponents();
  }
}

void FilterComponent::mouseUp (const MouseEvent& e)
{
  if (e.mouseWasClicked() && e.getNumberOfClicks() == 2)
  {
    if (const AudioProcessorGraph::Node::Ptr f = audioEngine.getDoc().getNodeForId (filterID))
    {
      AudioProcessor* const processor = f->getProcessor();
      String name = processor->getName();
      if(name != "Audio Input" && name != "Audio Output" && name != "Midi Input" && name != "Midi Output")
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
      audioEngine.getDoc().perform(new MoveFilterAction(audioEngine, *getGraphPanel(), filterID, startPos, endPos), "move filter");
    }
  }
}

bool FilterComponent::hitTest (int x, int y)
{
  for (int i = getNumChildComponents(); --i >= 0;)
    if (getChildComponent(i)->getBounds().contains (x, y))
      return true;
  
  return x >= 3 && x < getWidth() - 6 && y >= pinSize && y < getHeight() - pinSize;
}

void FilterComponent::paint (Graphics& g)
{
  g.setColour (Colours::lightgrey);
  
  const int x = 4;
  const int y = pinSize;
  const int w = getWidth() - x * 2;
  const int h = getHeight() - pinSize * 2;
  
  g.fillRoundedRectangle(x, y, w, h, 3);
  //    g.fillRect (x, y, w, h);
  
  g.setColour (Colours::black);
  //g.setFont (font);
  //g.drawFittedText (getName(), getLocalBounds().reduced (4, 2), Justification::centred, 2);
  
  if (getGraphPanel()->getLassoSelection().isSelected(this))
    g.setColour (Colours::red);
  else
    g.setColour (Colours::grey);
  //g.drawRect (x, y, w, h);
  g.drawRoundedRectangle(x, y, w, h, 3, 2);
}

void FilterComponent::resized()
{
  for (int i = 0; i < getNumChildComponents(); ++i)
  {
    if (PinComponent* const pc = dynamic_cast <PinComponent*> (getChildComponent(i)))
    {
      const int total = pc->isInput ? numIns : numOuts;
      const int index = pc->index == PMixDocument::midiChannelNumber ? (total - 1) : pc->index;
      
      pc->setBounds (proportionOfWidth ((1 + index) / (total + 1.0f)) - pinSize / 2,
                     pc->isInput ? 0 : (getHeight() - pinSize),
                     pinSize, pinSize);
    }
  }
  
  if (filterName != nullptr)
    filterName->setBounds(0, pinSize, getWidth(), 20);
  
  if (editor != nullptr)
    editor->setBounds(10, pinSize + filterName->getHeight(), getWidth()-20, getHeight() - pinSize - filterName->getHeight());
}

void FilterComponent::getPinPos (const int index, const bool isInput, float& x, float& y)
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

void FilterComponent::update()
{
  const AudioProcessorGraph::Node::Ptr f (audioEngine.getDoc().getNodeForId (filterID));
  
  if (f == nullptr)
  {
    delete this;
    return;
  }

  numIns = f->getProcessor()->getNumInputChannels();
  if (f->getProcessor()->acceptsMidi())
    ++numIns;
  
  numOuts = f->getProcessor()->getNumOutputChannels();
  if (f->getProcessor()->producesMidi())
    ++numOuts;
  
  int w = 80;
  int h = 50;

  w = jmax (w, (jmax (numIns, numOuts) + 1) * 20);
  
  const int textWidth = font.getStringWidth (f->getProcessor()->getName());
  w = jmax (w, 16 + jmin (textWidth, 300));
  if (textWidth > 300)
    h = 100;
  
  String name = f->getProcessor()->getName();
  
  setName (name);
  
  {
    double x, y;
    audioEngine.getDoc().getNodePosition (filterID, x, y);
    setCentreRelative ((float) x, (float) y);
  }

  if (numIns != numInputs || numOuts != numOutputs)
  {
    numInputs = numIns;
    numOutputs = numOuts;
    
    deleteAllChildren();
    
    addAndMakeVisible(filterName = new Label(name, name));
  filterName->setInterceptsMouseClicks(false, false);
  
  
    if(name != "Audio Input" && name != "Audio Output" && name != "Midi Input" && name != "Midi Output")
    {
      addAndMakeVisible(editor = new PMixGenericAudioProcessorEditor (f->getProcessor()));
      w = jmax (w, editor->getWidth() + 20 );
      h += editor->getHeight() + 10;
    }
    
    setSize (w, h);

    int i;
    for (i = 0; i < f->getProcessor()->getNumInputChannels(); ++i)
      addAndMakeVisible (new PinComponent (audioEngine, filterID, i, true));
    
    if (f->getProcessor()->acceptsMidi())
      addAndMakeVisible (new PinComponent (audioEngine, filterID, PMixDocument::midiChannelNumber, true));
    
    for (i = 0; i < f->getProcessor()->getNumOutputChannels(); ++i)
      addAndMakeVisible (new PinComponent (audioEngine, filterID, i, false));
    
    if (f->getProcessor()->producesMidi())
      addAndMakeVisible (new PinComponent (audioEngine, filterID, PMixDocument::midiChannelNumber, false));
    
    resized();
  }
}

GraphEditor* FilterComponent::getGraphPanel() const noexcept
{
  return findParentComponentOfClass<GraphEditor>();
}

#pragma mark -
#pragma mark ConnectorComponent

ConnectorComponent::ConnectorComponent (PMixAudioEngine& audioEngine)
: sourceFilterID (0),
destFilterID (0),
sourceFilterChannel (0),
destFilterChannel (0),
audioEngine (audioEngine),
lastInputX (0),
lastInputY (0),
lastOutputX (0),
lastOutputY (0)
{
  setAlwaysOnTop (true);
}

void ConnectorComponent::setInput (const uint32 sourceFilterID_, const int sourceFilterChannel_)
{
  if (sourceFilterID != sourceFilterID_ || sourceFilterChannel != sourceFilterChannel_)
  {
    sourceFilterID = sourceFilterID_;
    sourceFilterChannel = sourceFilterChannel_;
    update();
  }
}

void ConnectorComponent::setOutput (const uint32 destFilterID_, const int destFilterChannel_)
{
  if (destFilterID != destFilterID_ || destFilterChannel != destFilterChannel_)
  {
    destFilterID = destFilterID_;
    destFilterChannel = destFilterChannel_;
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
  
  if (GraphEditor* const hostPanel = getGraphPanel())
  {
    if (FilterComponent* srcFilterComp = hostPanel->getComponentForFilter (sourceFilterID))
      srcFilterComp->getPinPos (sourceFilterChannel, false, x1, y1);
    
    if (FilterComponent* dstFilterComp = hostPanel->getComponentForFilter (destFilterID))
      dstFilterComp->getPinPos (destFilterChannel, true, x2, y2);
  }
}

void ConnectorComponent::paint (Graphics& g)
{
  if (sourceFilterChannel == PMixDocument::midiChannelNumber
      || destFilterChannel == PMixDocument::midiChannelNumber)
  {
    g.setColour (Colours::red);
  }
  else
  {
    g.setColour (Colours::green);
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
    
    audioEngine.getDoc().removeConnection (sourceFilterID, sourceFilterChannel, destFilterID, destFilterChannel);
    
    double distanceFromStart, distanceFromEnd;
    getDistancesFromEnds (e.x, e.y, distanceFromStart, distanceFromEnd);
    const bool isNearerSource = (distanceFromStart < distanceFromEnd);
    
    getGraphPanel()->beginConnectorDrag (isNearerSource ? 0 : sourceFilterID,
                                         sourceFilterChannel,
                                         isNearerSource ? destFilterID : 0,
                                         destFilterChannel,
                                         e);
  }
  else if (dragging)
  {
    getGraphPanel()->dragConnector (e);
  }
}

void ConnectorComponent::mouseUp (const MouseEvent& e)
{
  if (dragging)
    getGraphPanel()->endDraggingConnector (e);
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
  linePath.cubicTo (x1, y1 + (y2 - y1) * 0.33f,
                    x2, y1 + (y2 - y1) * 0.66f,
                    x2, y2);
  
  PathStrokeType wideStroke (8.0f);
  wideStroke.createStrokedPath (hitPath, linePath);
  
  PathStrokeType stroke (2.5f);
  //stroke.createStrokedPath (linePath, linePath);
  float dashes[2] = { 4, 4 };
  stroke.createDashedStroke(linePath, linePath, dashes, 2);
  
  const float arrowW = 5.0f;
  const float arrowL = 4.0f;
  
  Path arrow;
  arrow.addTriangle (-arrowL, arrowW,
                     -arrowL, -arrowW,
                     arrowL, 0.0f);
  
  arrow.applyTransform (AffineTransform::identity
                        .rotated (float_Pi * 0.5f - (float) atan2 (x2 - x1, y2 - y1))
                        .translated ((x1 + x2) * 0.5f,
                                     (y1 + y2) * 0.5f));
  
  linePath.addPath (arrow);
  linePath.setUsingNonZeroWinding (true);
}

GraphEditor* ConnectorComponent::getGraphPanel() const noexcept
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