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

InterpolationSpacePreset::InterpolationSpacePreset(PMixAudioEngine& audioEngine, String& initalLabel, const uint32 filterID, const uint32 presetIdx, Colour colour)
: audioEngine(audioEngine)
, filterID(filterID)
, presetIdx(presetIdx)
, colour(colour)
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
  myDragger.startDraggingComponent (this, e);
  toFront (true);
  startBounds = getBounds();
  dynamic_cast<PMixInterpolationSpaceLayout*>(getParentComponent())->getLassoSelection().selectOnly(this);
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
  
  g.fillEllipse (5.f, 5.f, getWidth()-10.f, getHeight()-10.f);
  
  g.setColour(Colours::black);
  
  g.drawEllipse((getWidth()/2.f) - 5.f, (getHeight()/2.f)  - 5.f, 10.f, 10.f, 1.f);
  
  if (dynamic_cast<PMixInterpolationSpaceLayout*>(getParentComponent())->getLassoSelection().isSelected(this))
  {
    Path linePath;
    linePath.addEllipse (0, 0, getWidth(), getHeight());
    PathStrokeType stroke (2.5f);
    float dashes[2] = { 4, 4 };
    stroke.createDashedStroke(linePath, linePath, dashes, 2);
    g.setColour(Colours::lightgrey);
    g.fillPath (linePath);
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
  g.fillAll (Colours::white);
}

void PMixInterpolationSpaceLayout::mouseDown (const MouseEvent& e)
{
  selectedItems.deselectAll();
  addChildComponent (lassoComp);
  lassoComp.beginLasso (e, this);
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
  deleteAllChildren();
  
  for (int i = audioEngine.getDoc().getNumFilters(); --i >= 0;)
  {
    const AudioProcessorGraph::Node::Ptr f (audioEngine.getDoc().getNode (i));
    
    if (f->properties.getVarPointer("presets") != nullptr)
    {
      Array<var>* presets = f->properties.getVarPointer("presets")->getArray();
      
      for (int p=0; p< presets->size(); p++)
      {
        DynamicObject* obj = presets->getReference(p).getDynamicObject();
        
        String label = obj->getProperty("name");
        InterpolationSpacePreset* const comp = new InterpolationSpacePreset(audioEngine, label, f->nodeId, p, audioEngine.getDoc().getFilterColour(f->nodeId)  );
        String componentID;
        componentID << "p." << (int) f->nodeId << "." << p;
        comp->setComponentID(componentID);
        float r = 50. + (50. * (float) obj->getProperty("r"));
        float x = getWidth() * (float) obj->getProperty("x");
        float y = getHeight() * (float) obj->getProperty("y");
        comp->setBounds(x, y, r, r);
        addAndMakeVisible (comp);
      }
    }
    
  }
}
