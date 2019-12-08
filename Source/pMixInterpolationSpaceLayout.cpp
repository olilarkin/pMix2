/*
  ==============================================================================

    pMixInterpolationSpaceLayout.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixInterpolationSpaceLayout.h"
#include "pMixConstants.h"
#include "pMixCommandIDs.h"

InterpolationSpaceLabel::InterpolationSpaceLabel(const String& labelText)
: Label(String(), labelText)
{
  setInterceptsMouseClicks(false, false);
  setFont (Font (13.f));
  setJustificationType (Justification::centred);
  setEditable (false, true, false);
  setColour (Label::textColourId, Colours::white);
  setColour (TextEditor::textColourId, Colours::white);
  setColour (TextEditor::backgroundColourId, Colour (0x00000000));
}

InterpolationSpacePreset::InterpolationSpacePreset(PMixAudioEngine& audioEngine, String& initalLabel, NodeID nodeID, int presetId, Colour colour)
: audioEngine(audioEngine)
, nodeID(nodeID)
, presetId(presetId)
, colour(colour)
, opacity(1.)
, dragging(false)
{
  label = std::make_unique<InterpolationSpaceLabel>(initalLabel);
  addAndMakeVisible (*label);
  label->addListener(this);
}

InterpolationSpacePreset::~InterpolationSpacePreset ()
{
  label->removeListener(this);
}

void InterpolationSpacePreset::resized ()
{
  int radius = getWidth()/2;
  boundsConstrainer.setMinimumOnscreenAmounts(radius, radius, radius, radius);
  label->setBounds(0, 40., getWidth(), 20);
  //label->centreWithSize(getWidth()-5, 20);
}

void InterpolationSpacePreset::mouseDown (const MouseEvent& e)
{
  if (e.mods.isPopupMenu())
  {
    PopupMenu m;

    if (AudioProcessorGraph::Node::Ptr f = audioEngine.getDoc().getNodeForId (nodeID))
    {
      AudioProcessor* const processor = f->getProcessor();
      jassert (processor != nullptr);
      
      if(!InternalPluginFormat::isInternalFormat(processor->getName()))
      {
        m.addItem (1, TRANS("Delete Preset"));
        m.addItem (2, TRANS("Rename Preset"));
        m.addItem (3, TRANS("Update Preset"));
        m.addItem (4, TRANS("Randomize Preset"));
        m.addItem (5, TRANS("Set Colour"));
      }
    }
    
    const int r = m.show();
    
    if (r == 1)
    {
      audioEngine.getDoc().removePreset(nodeID, presetId);
    }
    else if (r == 2)
    {
      label->showEditor();
    }
    else if (r == 3)
    {
//      ColourSelector* colourSelector = new ColourSelector(ColourSelector::showSliders|ColourSelector::showColourAtTop|ColourSelector::showColourspace);
//      colourSelector->setName ("background");
//      colourSelector->setCurrentColour (audioEngine.getDoc().getNodeColour(nodeID));
//      colourSelector->addChangeListener (this);
//      colourSelector->setColour (ColourSelector::backgroundColourId, Colours::lightgrey);
//      colourSelector->setSize (300, 400);
//      
//      CallOutBox::launchAsynchronously (colourSelector, getScreenBounds(), nullptr);
    }
  }
  else
  {
    myDragger.startDraggingComponent (this, e);
    toFront (true);
    startBounds = getBounds();
    dragging = true;
    dynamic_cast<PMixInterpolationSpaceLayout*>(getParentComponent())->getLassoSelection().selectOnly(this);
  }
}

void InterpolationSpacePreset::mouseDrag (const MouseEvent& e)
{
  myDragger.dragComponent (this, e, &boundsConstrainer);  
}

void InterpolationSpacePreset::mouseUp (const MouseEvent& e)
{
  if (! e.mouseWasClicked())
  {
    endBounds = getBounds();
    
    if(dragging && startBounds != endBounds)
    {
      audioEngine.getDoc().beginTransaction();
      audioEngine.getDoc().perform(new MovePresetAction(dynamic_cast<PMixInterpolationSpaceLayout*>(getParentComponent()), getComponentID(), startBounds, endBounds), TRANS("change preset bounds"));
    }
  }
  
  dragging = false;
}

void InterpolationSpacePreset::paint (Graphics& g)
{
  g.setColour(colour);
  g.setOpacity(opacity);
  g.fillEllipse (5.f, 5.f, getWidth()-10.f, getHeight()-10.f);
  
  g.setColour(colour);
  g.fillEllipse ((getWidth()/2.f) - 5.f, (getHeight()/2.f)  - 5.f, 10.f, 10.f);
  g.setColour(Colours::black);
  g.drawEllipse((getWidth()/2.f) - 5.f, (getHeight()/2.f)  - 5.f, 10.f, 10.f, 1.f);
  
  if (dynamic_cast<PMixInterpolationSpaceLayout*>(getParentComponent())->getLassoSelection().isSelected(this))
  {
    Path linePath;
    linePath.addEllipse (2, 2, getWidth()-4, getHeight()-4);
    PathStrokeType stroke (2.5f);
    float dashes[2] = { 4, 4 };
    stroke.createDashedStroke(linePath, linePath, dashes, 2);
    g.setColour(Colours::lightgrey);
    g.fillPath (linePath);
  }
}

void InterpolationSpacePreset::update()
{
  const AudioProcessorGraph::Node::Ptr f (audioEngine.getDoc().getNodeForId (nodeID));
  
  if (f == nullptr)
  {
    delete this;
    return;
  }
  
  colour = audioEngine.getDoc().getNodeColour(nodeID);
  opacity = audioEngine.getDoc().getPresetWeight(nodeID, presetId);
  
  repaint();
}

void InterpolationSpacePreset::changeListenerCallback (ChangeBroadcaster* source)
{
  if (ColourSelector* cs = dynamic_cast <ColourSelector*> (source))
  {
    audioEngine.getDoc().setNodeColour(nodeID, cs->getCurrentColour());
  }
}

void InterpolationSpacePreset::labelTextChanged (Label* labelThatHasChanged)
{
  audioEngine.getDoc().setPresetName(nodeID, presetId, labelThatHasChanged->getText());
}

PMixInterpolationSpaceLayout::PMixInterpolationSpaceLayout(PMixAudioEngine& audioEngine, GraphEditor& graphEditor)
: audioEngine(audioEngine)
, graphEditor(graphEditor)
{
  audioEngine.getDoc().addChangeListener (this);
  selectedItems.addChangeListener(this);
  setMouseClickGrabsKeyboardFocus(true);
  setWantsKeyboardFocus(true);
  
#if PMIX_PLUGIN==0
  getCommandManager().registerAllCommandsForTarget (this);
#endif
}

PMixInterpolationSpaceLayout::~PMixInterpolationSpaceLayout()
{
  audioEngine.getDoc().removeChangeListener(this);
  selectedItems.removeChangeListener(this);
  deleteAllChildren();
}

void PMixInterpolationSpaceLayout::resized ()
{
}

void PMixInterpolationSpaceLayout::paint (Graphics& g)
{
}

void PMixInterpolationSpaceLayout::mouseDown (const MouseEvent& e)
{
  selectedItems.deselectAll();
  
  if (e.mods.isPopupMenu())
  {
    if(graphEditor.getLassoSelection().getNumSelected() == 1)
    {
      NodeComponent* selectedItem = dynamic_cast<NodeComponent*>(graphEditor.getLassoSelection().getSelectedItem(0));
      
      if (selectedItem)
      {
        AudioProcessor* proc = audioEngine.getDoc().getNodeForId(selectedItem->nodeID)->getProcessor();
        
        PopupMenu m;
        
        bool hasParams = (proc->getNumParameters() > 0);

        m.addItem (1, TRANS("Add preset for node"), hasParams);
        
        const int r = m.show();
        
        if (r == 1)
        {
          if (!InternalPluginFormat::isInternalFormat(proc->getName()))
          {
            double x = (double) e.getMouseDownX()/getWidth();
            double y = (double) e.getMouseDownY()/getHeight();

            audioEngine.getDoc().addPreset(selectedItem->nodeID, x, y);
          }
        }
      }
    }
  }
  else
  {
    addChildComponent (lassoComp);
    lassoComp.beginLasso (e, this);
  }
}

void PMixInterpolationSpaceLayout::mouseDrag (const MouseEvent& e)
{
  lassoComp.toFront (false);
  lassoComp.dragLasso (e);
}

void PMixInterpolationSpaceLayout::mouseUp (const MouseEvent& e)
{
  lassoComp.endLasso();
  removeChildComponent (&lassoComp);
}

void PMixInterpolationSpaceLayout::mouseDoubleClick (const MouseEvent& e)
{
  if(graphEditor.getLassoSelection().getNumSelected() == 1)
  {
    NodeComponent* selectedItem = dynamic_cast<NodeComponent*>(graphEditor.getLassoSelection().getSelectedItem(0));
    
    if (selectedItem)
    {
      AudioProcessor* proc = audioEngine.getDoc().getNodeForId(selectedItem->nodeID)->getProcessor();
      
      bool hasParams = (proc->getNumParameters() > 0);

      if (hasParams)
      {
        if (!InternalPluginFormat::isInternalFormat(proc->getName()))
        {
          double x = (double) e.getMouseDownX()/getWidth();
          double y = (double) e.getMouseDownY()/getHeight();
          
          audioEngine.getDoc().addPreset(selectedItem->nodeID, x, y);
        }
      }
    }
  }
}

//LassoSource
void PMixInterpolationSpaceLayout::findLassoItemsInArea (Array <Component*>& results, const Rectangle<int>& area)
{
  for (int i = 0; i < this->getNumChildComponents(); ++i)
  {
    Component* c = this->getChildComponent (i);
    
    if (c->getBounds().intersects (area))
      results.add (c);
  }
}

SelectedItemSet <Component*>& PMixInterpolationSpaceLayout::getLassoSelection()
{
  return selectedItems;
}

void PMixInterpolationSpaceLayout::changeListenerCallback (ChangeBroadcaster* source)
{
  if (source == &selectedItems)
  {
  }
  else
    updateComponents();
  
  repaint();
}

void PMixInterpolationSpaceLayout::updateComponents()
{
//  for (int i = getNumChildComponents(); --i >= 0;)
//  {
//    if (InterpolationSpacePreset* const pc = dynamic_cast <InterpolationSpacePreset*> (getChildComponent (i)))
//      pc->update();
//  }
  
  for (int i = audioEngine.getDoc().getNumNodes(); --i >= 0;)
  {
    const AudioProcessorGraph::Node::Ptr f (audioEngine.getDoc().getNode (i));
    
    if (!InternalPluginFormat::isInternalFormat(f->getProcessor()->getName()))
    {
      Array<InterpolationSpacePreset*> comps;
      getComponentsForNode(NodeID(f->nodeID), comps);
      Array<var>* presets = f->properties.getVarPointer("presets")->getArray();
      
      // if the number of presets for this node has changed then delete the components and re-create
      if (comps.size() != presets->size())
      {
        for (int componentIdx = 0; componentIdx<comps.size(); componentIdx++)
        {
          removeChildComponent(comps[componentIdx]);
          delete comps[componentIdx];
        }
        
        for (int presetIdx = 0; presetIdx < presets->size(); presetIdx++)
        {
          DynamicObject* obj = presets->getReference(presetIdx).getDynamicObject();
          
          String label = obj->getProperty("name");
          InterpolationSpacePreset* const comp = new InterpolationSpacePreset(audioEngine, label, f->nodeID, obj->getProperty("uid"), audioEngine.getDoc().getNodeColour(f->nodeID)  );
          String componentID;
          componentID << "p." << String(f->nodeID.uid) << "." << presetIdx;
          comp->setComponentID(componentID);
          float r = MIN_RADIUS + (RADIUS_RANGE * (float) obj->getProperty("radius"));
          float x = getWidth() * (float) obj->getProperty("x");
          float y = getHeight() * (float) obj->getProperty("y");
          
          comp->setSize(r, r);
          comp->setCentrePosition(x, y);
          comp->update();
          
          addAndMakeVisible (comp);
        }
      }
    }
    
  }
}

void PMixInterpolationSpaceLayout::getComponentsForNode (NodeID nodeID, Array<InterpolationSpacePreset*>& components) const
{
  for (int i = getNumChildComponents(); --i >= 0;)
  {
    if (InterpolationSpacePreset* const pc = dynamic_cast <InterpolationSpacePreset*> (getChildComponent (i)))
      if (pc->nodeID == nodeID)
        components.add(pc);
  }
}

void PMixInterpolationSpaceLayout::repaintPresetsForNode (NodeID nodeID)
{
  const AudioProcessorGraph::Node::Ptr f (audioEngine.getDoc().getNodeForId(nodeID));

  Array<InterpolationSpacePreset*> comps;
  getComponentsForNode(f->nodeID, comps);
  
  for (int i=0; i<comps.size(); i++) {
    comps[i]->update();
  }
}

#pragma mark ApplicationCommandTarget

ApplicationCommandTarget* PMixInterpolationSpaceLayout::getNextCommandTarget()
{
  return findFirstTargetParentComponent();
}

void PMixInterpolationSpaceLayout::getAllCommands (Array <CommandID>& commands)
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

void PMixInterpolationSpaceLayout::getCommandInfo (const CommandID commandID, ApplicationCommandInfo& result)
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

bool PMixInterpolationSpaceLayout::perform (const InvocationInfo& info)
{  
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

void PMixInterpolationSpaceLayout::deleteSelection()
{
  for (int i = 0; i < selectedItems.getNumSelected(); i++)
  {
    Component* c = selectedItems.getSelectedItem(i);
    
    InterpolationSpacePreset* p = dynamic_cast<InterpolationSpacePreset*>(c);
    
    if (p)
    {
      audioEngine.getDoc().removePreset(p->nodeID, p->presetId);
    }
  }
  
  selectedItems.deselectAll();
  
  updateComponents();
}

void PMixInterpolationSpaceLayout::selectAll()
{
  for (int i = 0; i < getNumChildComponents(); i++)
  {
    selectedItems.addToSelection(getChildComponent(i));
  }
}
