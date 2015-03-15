/*
  ==============================================================================

    pMixInterpolationSpaceLayout.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixInterpolationSpaceLayout.h"

InterpolationSpaceLabel::InterpolationSpaceLabel(const String& labelText)
: Label(String::empty, labelText)
{
  setInterceptsMouseClicks(false, false);
  setFont (Font (13.f));
  setJustificationType (Justification::centred);
  setEditable (false, true, false);
  setColour (Label::textColourId, Colours::white);
  setColour (TextEditor::textColourId, Colours::white);
  setColour (TextEditor::backgroundColourId, Colour (0x00000000));
}

InterpolationSpacePreset::InterpolationSpacePreset(PMixAudioEngine& audioEngine, String& initalLabel, const uint32 nodeId, const uint32 presetIdx, Colour colour)
: audioEngine(audioEngine)
, nodeId(nodeId)
, presetIdx(presetIdx)
, colour(colour)
, opacity(1.)
{
  //addAndMakeVisible (label = new InterpolationSpaceLabel (initalLabel));
}

InterpolationSpacePreset::~InterpolationSpacePreset ()
{
}

void InterpolationSpacePreset::resized ()
{
  int radius = getWidth()/2;
  boundsConstrainer.setMinimumOnscreenAmounts(radius,radius,radius,radius);
  //  label->centreWithSize(getWidth()-5, 20);
}

void InterpolationSpacePreset::mouseDown (const MouseEvent& e)
{
  if (e.mods.isPopupMenu())
  {
    PopupMenu m;

    if (AudioProcessorGraph::Node::Ptr f = audioEngine.getDoc().getNodeForId (nodeId))
    {
      AudioProcessor* const processor = f->getProcessor();
      jassert (processor != nullptr);
      
      if(!InternalPluginFormat::isInternalFormat(processor->getName()))
      {
        m.addItem (1, "Delete Preset");
        m.addItem (2, "Rename Preset");
        m.addItem (3, "Update Preset");
        m.addItem (4, "Randomize Preset");
        m.addItem (5, "Set Colour");
      }
    }
    
    const int r = m.show();
    
    if (r == 1)
    {
      audioEngine.getDoc().removePreset(nodeId, presetIdx);
    }
    else if (r == 2)
    {
      
    }
    else if (r == 3)
    {
//      ColourSelector* colourSelector = new ColourSelector(ColourSelector::showSliders|ColourSelector::showColourAtTop|ColourSelector::showColourspace);
//      colourSelector->setName ("background");
//      colourSelector->setCurrentColour (audioEngine.getDoc().getFilterColour(nodeId));
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
    dynamic_cast<PMixInterpolationSpaceLayout*>(getParentComponent())->getLassoSelection().selectOnly(this);
  }
}

void InterpolationSpacePreset::mouseDrag (const MouseEvent& e)
{
  myDragger.dragComponent (this, e, &boundsConstrainer);
}

void InterpolationSpacePreset::mouseUp (const MouseEvent& e)
{
  endBounds = getBounds();
  
  audioEngine.getDoc().beginTransaction();
  audioEngine.getDoc().perform(new MovePresetAction(dynamic_cast<PMixInterpolationSpaceLayout*>(getParentComponent()), getComponentID(), startBounds, endBounds), "change preset bounds");
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
  const AudioProcessorGraph::Node::Ptr f (audioEngine.getDoc().getNodeForId (nodeId));
  
  if (f == nullptr)
  {
    delete this;
    return;
  }
  
  colour = audioEngine.getDoc().getFilterColour(nodeId);
  opacity = audioEngine.getDoc().getPresetWeight(nodeId, presetIdx);
  
  repaint();
}

void InterpolationSpacePreset::changeListenerCallback (ChangeBroadcaster* source)
{
  if (ColourSelector* cs = dynamic_cast <ColourSelector*> (source))
  {
    audioEngine.getDoc().setFilterColour(nodeId, cs->getCurrentColour());
  }
}

PMixInterpolationSpaceLayout::PMixInterpolationSpaceLayout(PMixAudioEngine& audioEngine)
: audioEngine(audioEngine)
{
  audioEngine.getDoc().addChangeListener (this);
  selectedItems.addChangeListener(this);
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
    PopupMenu m;
    
    m.addItem (1, "Add preset for filter");
    
    m.show();
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
  for (int i = getNumChildComponents(); --i >= 0;)
  {
    if (InterpolationSpacePreset* const pc = dynamic_cast <InterpolationSpacePreset*> (getChildComponent (i)))
      pc->update();
  }
  
  for (int i = audioEngine.getDoc().getNumFilters(); --i >= 0;)
  {
    const AudioProcessorGraph::Node::Ptr f (audioEngine.getDoc().getNode (i));
    
    if (!InternalPluginFormat::isInternalFormat(f->getProcessor()->getName()))
    {
      Array<InterpolationSpacePreset*> comps;
      getComponentsForFilter(f->nodeId, comps);
      Array<var>* presets = f->properties.getVarPointer("presets")->getArray();
      
      // if the number of presets for this filter has changed then delete the components and re-create
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
          InterpolationSpacePreset* const comp = new InterpolationSpacePreset(audioEngine, label, f->nodeId, presetIdx, audioEngine.getDoc().getFilterColour(f->nodeId)  );
          String componentID;
          componentID << "p." << (int) f->nodeId << "." << presetIdx;
          comp->setComponentID(componentID);
          float r = 50.f + (100.f * (float) obj->getProperty("radius"));
          float x = getWidth() * (float) obj->getProperty("x");
          float y = getHeight() * (float) obj->getProperty("y");
          comp->setBounds(x, y, r, r);
          addAndMakeVisible (comp);
        }
      }
    }
    
  }
}

void PMixInterpolationSpaceLayout::getComponentsForFilter (const uint32 nodeId, Array<InterpolationSpacePreset*>& components) const
{
  for (int i = getNumChildComponents(); --i >= 0;)
  {
    if (InterpolationSpacePreset* const pc = dynamic_cast <InterpolationSpacePreset*> (getChildComponent (i)))
      if (pc->nodeId == nodeId)
        components.add(pc);
  }
}
