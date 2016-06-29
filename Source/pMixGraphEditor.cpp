/*
  ==============================================================================

    pMixGraphEditor.cpp
    based on JUCE demo host
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixGraphEditor.h"
#include "pMixCommandIDs.h"
#include "pMixGraphEditorActions.h"

GraphEditor::GraphEditor (PMixAudioEngine& audioEngine)
  : audioEngine (audioEngine)
  , somethingIsBeingDraggedOver (false)
{
  audioEngine.getDoc().addChangeListener (this);
  selectedItems.addChangeListener(this);
  setWantsKeyboardFocus(true);
  
#if PMIX_PLUGIN==0
  getCommandManager().registerAllCommandsForTarget (this);
#endif
  
  setOpaque (true);
}

GraphEditor::~GraphEditor()
{
  audioEngine.getDoc().removeChangeListener (this);
  selectedItems.removeChangeListener (this);
  draggingConnector = nullptr;
  deleteAllChildren();
}

void GraphEditor::paint (Graphics& g)
{
  g.fillAll (Colours::white);
  
  if (somethingIsBeingDraggedOver)
  {
    g.setColour (Colours::red);
    g.drawRect (getLocalBounds(), 3);
  }
}

void GraphEditor::mouseDown (const MouseEvent& e)
{
  if (e.mods.isPopupMenu())
  {
    PopupMenu m;
    audioEngine.createNodeMenu(m);
    const int r = m.show();
    createNewNode (audioEngine.getChosenType (r), e.x, e.y);
  }
  else
  {
    addChildComponent (lassoComp);
    lassoComp.beginLasso (e, this);
  }
}

void GraphEditor::mouseDoubleClick (const MouseEvent&)
{
  selectedItems.deselectAll();
}

void GraphEditor::mouseDrag (const MouseEvent& e)
{
  lassoComp.toFront (false);
  lassoComp.dragLasso (e);
}

void GraphEditor::mouseUp (const MouseEvent& e)
{
  lassoComp.endLasso();
  removeChildComponent (&lassoComp);
}

void GraphEditor::createNewNode (const PluginDescription* desc, int x, int y)
{
  if (desc != nullptr)
  {
    audioEngine.getDoc().beginTransaction();
    audioEngine.getDoc().perform(new CreateNodeAction(audioEngine, *this, desc, x / (double) getWidth(), y / (double) getHeight()), TRANS("add node"));
  }
}

NodeComponent* GraphEditor::getComponentForNode (const uint32 nodeId) const
{
  for (int i = getNumChildComponents(); --i >= 0;)
  {
    if (NodeComponent* const fc = dynamic_cast <NodeComponent*> (getChildComponent (i)))
      if (fc->nodeId == nodeId)
        return fc;
  }

  return nullptr;
}

ConnectorComponent* GraphEditor::getComponentForConnection (const AudioProcessorGraph::Connection& conn) const
{
  for (int i = getNumChildComponents(); --i >= 0;)
  {
    if (ConnectorComponent* const c = dynamic_cast <ConnectorComponent*> (getChildComponent (i)))
      if (c->sourceNodeId == conn.sourceNodeId
          && c->destNodeId == conn.destNodeId
          && c->sourceNodeChannel == conn.sourceChannelIndex
          && c->destNodeChannel == conn.destChannelIndex)
        return c;
  }

  return nullptr;
}

PinComponent* GraphEditor::findPinAt (const int x, const int y) const
{
  for (int i = getNumChildComponents(); --i >= 0;)
  {
    if (NodeComponent* fc = dynamic_cast <NodeComponent*> (getChildComponent (i)))
    {
      if (PinComponent* pin = dynamic_cast <PinComponent*> (fc->getComponentAt (x - fc->getX(),
                              y - fc->getY())))
        return pin;
    }
  }

  return nullptr;
}

void GraphEditor::resized()
{
  updateComponents();
}

void GraphEditor::changeListenerCallback (ChangeBroadcaster* source)
{
  repaint();

  if (source == &selectedItems)
  {
    sendChangeMessage();
  }
  else
    updateComponents();
}

void GraphEditor::updateComponents()
{
  for (int i = getNumChildComponents(); --i >= 0;)
  {
    if (NodeComponent* const fc = dynamic_cast <NodeComponent*> (getChildComponent (i)))
      fc->update();
  }

  for (int i = getNumChildComponents(); --i >= 0;)
  {
    ConnectorComponent* const cc = dynamic_cast <ConnectorComponent*> (getChildComponent (i));

    if (cc != nullptr && cc != draggingConnector)
    {
      if (audioEngine.getDoc().getConnectionBetween (cc->sourceNodeId, cc->sourceNodeChannel,
                                      cc->destNodeId, cc->destNodeChannel) == nullptr)
      {
        delete cc;
      }
      else
      {
        cc->update();
      }
    }
  }

  for (int i = audioEngine.getDoc().getNumNodes(); --i >= 0;)
  {
    const AudioProcessorGraph::Node::Ptr f (audioEngine.getDoc().getNode (i));

    if (getComponentForNode (f->nodeId) == 0)
    {
      NodeComponent* const comp = new NodeComponent (audioEngine, f->nodeId);
      addAndMakeVisible (comp);
      comp->update();
    }
  }

  for (int i = audioEngine.getDoc().getNumConnections(); --i >= 0;)
  {
    const AudioProcessorGraph::Connection* const c = audioEngine.getDoc().getConnection (i);

    if (getComponentForConnection (*c) == 0)
    {
      ConnectorComponent* const comp = new ConnectorComponent (audioEngine);
      addAndMakeVisible (comp);

      comp->setInput (c->sourceNodeId, c->sourceChannelIndex);
      comp->setOutput (c->destNodeId, c->destChannelIndex);
    }
  }
}

void GraphEditor::beginConnectorDrag (const uint32 sourceNodeId, const int sourceNodeChannel,
    const uint32 destNodeId, const int destNodeChannel,
    const MouseEvent& e)
{
  draggingConnector = dynamic_cast <ConnectorComponent*> (e.originalComponent);

  if (draggingConnector == nullptr)
    draggingConnector = new ConnectorComponent (audioEngine);

  draggingConnector->setInput (sourceNodeId, sourceNodeChannel);
  draggingConnector->setOutput (destNodeId, destNodeChannel);

  addAndMakeVisible (draggingConnector);
  draggingConnector->toFront (false);

  dragConnector (e);
}

void GraphEditor::dragConnector (const MouseEvent& e)
{
  const MouseEvent e2 (e.getEventRelativeTo (this));

  if (draggingConnector != nullptr)
  {
    draggingConnector->setTooltip (String::empty);

    int x = e2.x;
    int y = e2.y;

    if (PinComponent* const pin = findPinAt (x, y))
    {
      uint32 srcNode = draggingConnector->sourceNodeId;
      int srcChannel   = draggingConnector->sourceNodeChannel;
      uint32 dstNode = draggingConnector->destNodeId;
      int dstChannel   = draggingConnector->destNodeChannel;

      if (srcNode == 0 && ! pin->isInput)
      {
        srcNode = pin->nodeId;
        srcChannel = pin->index;
      }
      else if (dstNode == 0 && pin->isInput)
      {
        dstNode = pin->nodeId;
        dstChannel = pin->index;
      }

      if (audioEngine.getDoc().canConnect (srcNode, srcChannel, dstNode, dstChannel))
      {
        pin->mouseOver = true;
        pin->repaint();

        x = pin->getParentComponent()->getX() + pin->getX() + pin->getWidth() / 2;
        y = pin->getParentComponent()->getY() + pin->getY() + pin->getHeight() / 2;

        draggingConnector->setTooltip (pin->getTooltip());
      }
    }

    if (draggingConnector->sourceNodeId == 0)
      draggingConnector->dragStart (x, y);
    else
      draggingConnector->dragEnd (x, y);
  }
}

void GraphEditor::endDraggingConnector (const MouseEvent& e)
{
  if (draggingConnector == nullptr)
    return;

  draggingConnector->setTooltip (String::empty);

  const MouseEvent e2 (e.getEventRelativeTo (this));

  uint32 srcNode = draggingConnector->sourceNodeId;
  int srcChannel   = draggingConnector->sourceNodeChannel;
  uint32 dstNode = draggingConnector->destNodeId;
  int dstChannel   = draggingConnector->destNodeChannel;

  draggingConnector = nullptr;

  if (PinComponent* const pin = findPinAt (e2.x, e2.y))
  {
    if (srcNode == 0)
    {
      if (pin->isInput)
        return;

      srcNode = pin->nodeId;
      srcChannel = pin->index;
    }
    else
    {
      if (! pin->isInput)
        return;

      dstNode = pin->nodeId;
      dstChannel = pin->index;
    }

    audioEngine.getDoc().beginTransaction();
    audioEngine.getDoc().perform(new CreateConnectionAction(audioEngine, srcNode, srcChannel, dstNode, dstChannel), TRANS("add connection"));
  }
}


#pragma mark ApplicationCommandTarget

ApplicationCommandTarget* GraphEditor::getNextCommandTarget()
{
  return findFirstTargetParentComponent();
}

void GraphEditor::getAllCommands (Array <CommandID>& commands)
{
  // this returns the set of all commands that this target can perform..
  const CommandID ids[] = {
    CommandIDs::copy ,
    CommandIDs::paste ,
    CommandIDs::del ,
    CommandIDs::selectAll ,
    CommandIDs::zoomIn ,
    CommandIDs::zoomOut ,
    CommandIDs::zoomNormal
  };
  
  commands.addArray (ids, numElementsInArray (ids));
}

void GraphEditor::getCommandInfo (const CommandID commandID, ApplicationCommandInfo& result)
{
  const String category (TRANS("General"));
  
  switch (commandID)
  {
    case CommandIDs::copy:
      result.setInfo (TRANS("Copy"), TRANS("Copies the currently selected items to the clipboard"), category, 0);
      result.defaultKeypresses.add (KeyPress ('c', ModifierKeys::commandModifier, 0));
      break;
    case CommandIDs::paste:
      result.setInfo (TRANS("Paste"), TRANS("Pastes from the clipboard"), category, 0);
      result.defaultKeypresses.add (KeyPress ('v', ModifierKeys::commandModifier, 0));
      break;
    case CommandIDs::del:
      result.setInfo (TRANS("Delete"), TRANS("Deletes the selection"), category, 0);
      result.defaultKeypresses.add (KeyPress (KeyPress::backspaceKey, ModifierKeys::noModifiers, 0));
      break;
    case CommandIDs::selectAll:
      result.setInfo (TRANS("Select All"), TRANS("Select All"), category, 0);
      result.defaultKeypresses.add (KeyPress ('a', ModifierKeys::commandModifier, 0));
      break;
    case CommandIDs::zoomIn:
      result.setInfo (TRANS("Zoom in"), TRANS("Zooms in on the current component."), category, 0);
      result.defaultKeypresses.add (KeyPress (']', ModifierKeys::commandModifier, 0));
      break;
      
    case CommandIDs::zoomOut:
      result.setInfo (TRANS("Zoom out"), TRANS("Zooms out on the current component."), category, 0);
      result.defaultKeypresses.add (KeyPress ('[', ModifierKeys::commandModifier, 0));
      break;
      
    case CommandIDs::zoomNormal:
      result.setInfo (TRANS("Zoom to 100%"), TRANS("Restores the zoom level to normal."), category, 0);
      result.defaultKeypresses.add (KeyPress ('1', ModifierKeys::commandModifier, 0));
      break;
  }
}

bool GraphEditor::perform (const InvocationInfo& info)
{
//  MainComponent* const mainComponent = getMainComponent();
  
  switch (info.commandID)
  {
    case CommandIDs::copy:
      // TODO
      break;
      
    case CommandIDs::paste:
      // TODO
      break;
      
    case CommandIDs::del:
      deleteSelection();
      break;
    case CommandIDs::selectAll:
      selectAll();
      break;
      
//    case CommandIDs::zoomIn:      getMainComponent()->setZoom (snapToIntegerZoom (getMainComponent()->getZoom() * 2.0)); break;
//    case CommandIDs::zoomOut:     getMainComponent()->setZoom (snapToIntegerZoom (getMainComponent()->getZoom() / 2.0)); break;
//    case CommandIDs::zoomNormal:  getMainComponent()->setZoom (1.0); break;
      
    default:
      return false;
  }
  
  return true;
}


void GraphEditor::findLassoItemsInArea (Array <Component*>& results, const Rectangle<int>& area)
{
  const Rectangle<int> lasso (area - this->getPosition());
  
  for (int i = 0; i < this->getNumChildComponents(); ++i)
  {
    Component* c = this->getChildComponent (i);
    
    if (c->getBounds().intersects (lasso))
      results.add (c);
  }
}

SelectedItemSet <Component*>& GraphEditor::getLassoSelection()
{
  return selectedItems;
}

void GraphEditor::updateFaustNode (const uint32 nodeId, String& newSourceCode)
{
  ScopedPointer<XmlElement> temp(audioEngine.getDoc().createXml());
  const XmlElement tempXml (*temp);
  
  PluginWindow::closeCurrentlyOpenWindowsFor (nodeId);
  getComponentForNode(nodeId)->removeEditor();
  audioEngine.getDoc().removeNode(nodeId);
  
  forEachXmlChildElementWithTagName (tempXml, e, "NODE")
  {
    if(nodeId == e->getIntAttribute("uid"))
    {
      double origX = e->getDoubleAttribute("x");
      double origY = e->getDoubleAttribute("y");
      e->setAttribute("x", origX);
      e->setAttribute("y", origY);
      audioEngine.getDoc().createNodeFromXml(*e, newSourceCode);
    }
  }
  
  forEachXmlChildElementWithTagName (tempXml, e, "CONNECTION")
  {
    if(e->getIntAttribute ("srcNode") == nodeId || e->getIntAttribute ("dstNode") == nodeId)
    {
      audioEngine.getDoc().addConnection((uint32) e->getIntAttribute ("srcNode"), e->getIntAttribute ("srcChannel"), (uint32) e->getIntAttribute ("dstNode"), e->getIntAttribute ("dstChannel"));
    }
  }
    
  sendChangeMessage();
}

void GraphEditor::clear()
{
  for (int i = audioEngine.getDoc().getNumNodes(); --i >= 0;)
  {
    const AudioProcessorGraph::Node::Ptr f (audioEngine.getDoc().getNode (i));
    
    getComponentForNode (f->nodeId)->removeEditor();
  }
}

bool GraphEditor::isInterestedInFileDrag (const StringArray& files)
{
  if (files.size() == 1)
  {
    File theFile(files[0]);
    if (theFile.getFileExtension() == ".dsp")
      return true;
  }
  
  return false;
}

void GraphEditor::fileDragEnter (const StringArray& files, int x, int y)
{
  somethingIsBeingDraggedOver = true;

  repaint();
}

void GraphEditor::fileDragMove (const StringArray& files, int x, int y)
{
}

void GraphEditor::fileDragExit (const StringArray& files)
{
  somethingIsBeingDraggedOver = false;

  repaint();
}

void GraphEditor::filesDropped (const StringArray& files, int x, int y)
{
  somethingIsBeingDraggedOver = false;
  
  PluginDescription desc;
  FaustAudioPluginInstance::fillInitialInPluginDescription(desc);
  desc.fileOrIdentifier = files[0];
  audioEngine.getDoc().perform(new CreateNodeAction(audioEngine, *this, &desc, x / (double) getWidth(), y / (double) getHeight()), TRANS("add node"));

  repaint();
}

void GraphEditor::clearSelection()
{
  selectedItems.deselectAll();
  sendChangeMessage();
}

void GraphEditor::deleteSelection()
{
  for (int i = 0; i < selectedItems.getNumSelected(); i++)
  {
    Component* c = selectedItems.getSelectedItem(i);
    
    NodeComponent* fc = dynamic_cast<NodeComponent*>(c);
    
    if (fc)
    {
      audioEngine.getDoc().perform(new RemoveNodeAction(audioEngine, *this, fc->nodeId), TRANS("remove node"));
    }
  }
  
  selectedItems.deselectAll();
  updateComponents();
}

void GraphEditor::selectAll()
{
  for (int i = 0; i < getNumChildComponents(); i++)
  {
    selectedItems.addToSelection(getChildComponent(i));
  }
}
