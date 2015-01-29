#include "pMixInterpolationSpace.h"

InterpolationSpaceLabel::InterpolationSpaceLabel(const String& labelText) 
: Label(String::empty, labelText)
{
  setInterceptsMouseClicks(false, false);
}

MovePresetAction::MovePresetAction (Component* interpolationSpace, const String& componentID, Rectangle<int> startBounds, Rectangle<int> endBounds) noexcept
: interpolationSpace(interpolationSpace)
, componentID(componentID)
, startBounds(startBounds)
, endBounds(endBounds)
{
}

bool MovePresetAction::perform()
{
  interpolationSpace->findChildWithID(componentID)->setBounds(endBounds);
  return true;
}

bool MovePresetAction::undo()
{      
  interpolationSpace->findChildWithID(componentID)->setBounds(startBounds);
  return true;
}

int MovePresetAction::getSizeInUnits()
{
  return (int) sizeof (*this); //xxx should be more accurate
}

InterpolationSpacePreset::InterpolationSpacePreset(PMixAudio& audio, String& initalLabel)
: audio(audio)
{
  addAndMakeVisible (label = new InterpolationSpaceLabel (initalLabel));
  label->setFont (Font (13.00f));
  label->setJustificationType (Justification::centred);
  label->setEditable (false, true, false);
  label->setColour (Label::textColourId, Colours::white);
  label->setColour (TextEditor::textColourId, Colours::white);
  label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
}

InterpolationSpacePreset::~InterpolationSpacePreset ()
{
  deleteAllChildren();
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
  
  audio.getDoc().beginTransaction();
  audio.getDoc().perform(new MovePresetAction(getParentComponent(), getComponentID(), startBounds, endBounds), "change preset bounds");
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

InterpolationSpaceComponent::InterpolationSpaceComponent (PMixAudio& audio)
: audio (audio)
, mRand(Time::currentTimeMillis())
{
  for(int i = 0; i<10; i++)
  {
    String lab = String("preset") + String(i);
    InterpolationSpacePreset* const comp = new InterpolationSpacePreset(audio, lab);
    comp->setComponentID(lab);
    addAndMakeVisible (comp);
  }
  
  selectedItems.addChangeListener(this);
}

InterpolationSpaceComponent::~InterpolationSpaceComponent ()
{
  selectedItems.removeChangeListener(this);
  deleteAllChildren();
}

void InterpolationSpaceComponent::resized ()
{
  for (int i = 0; i < getNumChildComponents(); ++i)
  {
    InterpolationSpacePreset* const comp = dynamic_cast <InterpolationSpacePreset*> (getChildComponent(i));
    
    float r = 50. + (50. * mRand.nextFloat());
    float x = getWidth() * mRand.nextFloat();
    float y = getHeight() * mRand.nextFloat();
    comp->setBounds(x, y, r, r);
  }
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

void InterpolationSpaceComponent::changeListenerCallback (ChangeBroadcaster*)
{
  repaint();
}

