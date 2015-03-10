#include "pMixInterpolationSpace.h"

InterpolationSpaceLabel::InterpolationSpaceLabel(const String& labelText) 
: Label(String::empty, labelText)
{
  setInterceptsMouseClicks(false, false);
  setFont (Font (13.00f));
  setJustificationType (Justification::centred);
  setEditable (false, true, false);
  setColour (Label::textColourId, Colours::white);
  setColour (TextEditor::textColourId, Colours::white);
  setColour (TextEditor::backgroundColourId, Colour (0x00000000));
}

InterpolationSpacePreset::InterpolationSpacePreset(PMixAudioEngine& audioEngine, String& initalLabel)
: audioEngine(audioEngine)
{
  addAndMakeVisible (label = new InterpolationSpaceLabel (initalLabel));

}

InterpolationSpacePreset::~InterpolationSpacePreset ()
{
}

void InterpolationSpacePreset::resized ()
{
  int radius = getWidth()/2;
  boundsConstrainer.setMinimumOnscreenAmounts(radius,radius,radius,radius);
  label->centreWithSize(getWidth()-5, 20);
}

void InterpolationSpacePreset::mouseDown (const MouseEvent& e)
{
  myDragger.startDraggingComponent (this, e);
  toFront (true);
  startBounds = getBounds();
  dynamic_cast<InterpolationSpaceComponent*>(getParentComponent())->getLassoSelection().selectOnly(this);
}

void InterpolationSpacePreset::mouseDrag (const MouseEvent& e)
{
  myDragger.dragComponent (this, e, &boundsConstrainer);
}

void InterpolationSpacePreset::mouseUp (const MouseEvent& e)
{
  endBounds = getBounds();
  
  audioEngine.getDoc().beginTransaction();
  audioEngine.getDoc().perform(new MovePresetAction(getParentComponent(), getComponentID(), startBounds, endBounds), "change preset bounds");
}

void InterpolationSpacePreset::paint (Graphics& g)
{
  if (dynamic_cast<InterpolationSpaceComponent*>(getParentComponent())->getLassoSelection().isSelected(this))
    g.setColour(Colours::black);
  else
    g.setColour(Colours::red);
  g.fillEllipse (0, 0, getWidth(), getHeight());

  //g.setColour(Colours::white);
  //g.drawFittedText(getComponentID(), 0, 0, getWidth(), getHeight(), Justification::centred, 1);
}

InterpolationSpaceComponent::InterpolationSpaceComponent (PMixAudioEngine& audioEngine)
: audioEngine(audioEngine)
, mRand(Time::currentTimeMillis())
{
  audioEngine.getDoc().addChangeListener (this);
  selectedItems.addChangeListener(this);
}

InterpolationSpaceComponent::~InterpolationSpaceComponent ()
{
  audioEngine.getDoc().removeChangeListener(this);
  selectedItems.removeChangeListener(this);
  deleteAllChildren();
}

void InterpolationSpaceComponent::resized ()
{
}

void InterpolationSpaceComponent::paint (Graphics& g)
{
  g.fillAll (Colours::white);
}

void InterpolationSpaceComponent::mouseDown (const MouseEvent& e)
{
  selectedItems.deselectAll();
  addChildComponent (lassoComp);
  lassoComp.beginLasso (e, this);
}

void InterpolationSpaceComponent::mouseDrag (const MouseEvent& e)
{
  lassoComp.toFront (false);
  lassoComp.dragLasso (e);
}

void InterpolationSpaceComponent::mouseUp (const MouseEvent& e)
{
  lassoComp.endLasso();
  removeChildComponent (&lassoComp);
}

//LassoSource
void InterpolationSpaceComponent::findLassoItemsInArea (Array <Component*>& results, const Rectangle<int>& area)
{  
  for (int i = 0; i < this->getNumChildComponents(); ++i)
  {
    Component* c = this->getChildComponent (i);
    
    if (c->getBounds().intersects (area))
      results.add (c);
  }
}

SelectedItemSet <Component*>& InterpolationSpaceComponent::getLassoSelection()
{
  return selectedItems;
}

void InterpolationSpaceComponent::changeListenerCallback (ChangeBroadcaster* source)
{
  if (source == &selectedItems)
  {
  }
  else
    updateComponents();
  
  repaint();
}

void InterpolationSpaceComponent::updateComponents()
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

        String lab = obj->getProperty("name");
        InterpolationSpacePreset* const comp = new InterpolationSpacePreset(audioEngine, lab);
        comp->setComponentID(lab);
        float r = 50. + (50. * (float) obj->getProperty("r"));
        float x = getWidth() * (float) obj->getProperty("x");
        float y = getHeight() * (float) obj->getProperty("y");
        comp->setBounds(x, y, r, r);
        addAndMakeVisible (comp);
      }
    }
    
//    if (getComponentForFilter (f->nodeId) == 0)
//    {
//      FilterComponent* const comp = new FilterComponent (audioEngine, f->nodeId);
//      addAndMakeVisible (comp);
//      comp->update();
//    }
  }
  
//  for (int i = audioEngine.getDoc().getNumConnections(); --i >= 0;)
//  {
//    const AudioProcessorGraph::Connection* const c = audioEngine.getDoc().getConnection (i);
//    
//    if (getComponentForConnection (*c) == 0)
//    {
//      ConnectorComponent* const comp = new ConnectorComponent (audioEngine);
//      addAndMakeVisible (comp);
//      
//      comp->setInput (c->sourceNodeId, c->sourceChannelIndex);
//      comp->setOutput (c->destNodeId, c->destChannelIndex);
//    }
//  }
}

