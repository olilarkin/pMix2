#include "InterpolationSpace.h"

iSpaceLabel::iSpaceLabel(const String& labelText) 
: Label(String::empty, labelText)
{
}

MovePresetAction::MovePresetAction (Component* iSpace, const String& componentID, Rectangle<int> startBounds, Rectangle<int> endBounds) noexcept
: iSpace(iSpace)
, componentID(componentID)
, startBounds(startBounds)
, endBounds(endBounds)
{
}

bool MovePresetAction::perform()
{
  iSpace->findChildWithID(componentID)->setBounds(endBounds);
  return true;
}

bool MovePresetAction::undo()
{      
  iSpace->findChildWithID(componentID)->setBounds(startBounds);
  return true;
}

int MovePresetAction::getSizeInUnits()
{
  return (int) sizeof (*this); //xxx should be more accurate
}

iSpacePreset::iSpacePreset(UndoManager& undoManager, String& initalLabel)
: undoManager (undoManager)
{
  addAndMakeVisible (label = new iSpaceLabel (initalLabel));
  label->setFont (Font (13.00f));
  label->setJustificationType (Justification::centred);
  label->setEditable (false, true, false);
  label->setColour (Label::textColourId, Colours::white);
  label->setColour (TextEditor::textColourId, Colours::white);
  label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
}

iSpacePreset::~iSpacePreset ()
{
  deleteAllChildren();
}

void iSpacePreset::resized ()
{
  int radius = getWidth()/2;
  boundsConstrainer.setMinimumOnscreenAmounts(radius,radius,radius,radius);
  label->centreWithSize(getWidth()-5, 20);
}

void iSpacePreset::mouseDown (const MouseEvent& e)
{
  myDragger.startDraggingComponent (this, e);
  toFront (true);
  startBounds = getBounds();
}

void iSpacePreset::mouseDrag (const MouseEvent& e)
{
  myDragger.dragComponent (this, e, &boundsConstrainer);
}

void iSpacePreset::mouseUp (const MouseEvent& e)
{
  endBounds = getBounds();
  
  undoManager.beginNewTransaction();
  undoManager.perform(new MovePresetAction(getParentComponent(), getComponentID(), startBounds, endBounds), "change preset bounds");
}

void iSpacePreset::paint (Graphics& g)
{
  g.setColour(Colours::red);
  g.fillEllipse (0, 0, getWidth(), getHeight());
  //g.setColour(Colours::white);
  //g.drawFittedText(getComponentID(), 0, 0, getWidth(), getHeight(), Justification::centred, 1);
}

iSpaceComponent::iSpaceComponent (UndoManager& undoManager)
: undoManager (undoManager)
, mRand(Time::currentTimeMillis())
{
  for(int i = 0; i<10; i++)
  {
    String lab = String("preset") + String(i);
    iSpacePreset* const comp = new iSpacePreset(undoManager, lab);
    comp->setComponentID(lab);
    addAndMakeVisible (comp);
  }
}

iSpaceComponent::~iSpaceComponent ()
{
  deleteAllChildren();
}

void iSpaceComponent::resized ()
{
  for (int i = 0; i < getNumChildComponents(); ++i)
  {
    iSpacePreset* const comp = dynamic_cast <iSpacePreset*> (getChildComponent(i));
    
    float r = 50. + (50. * mRand.nextFloat());
    float x = getWidth() * mRand.nextFloat();
    float y = getHeight() * mRand.nextFloat();
    comp->setBounds(x, y, r, r);
  }
}

void iSpaceComponent::paint (Graphics& g)
{
  g.fillAll (Colours::white);
}

void iSpaceComponent::mouseDown (const MouseEvent& e)
{
  addChildComponent (lassoComp);
  lassoComp.beginLasso (e, this);
}

void iSpaceComponent::mouseDrag (const MouseEvent& e)
{
  lassoComp.toFront (false);
  lassoComp.dragLasso (e);
}

void iSpaceComponent::mouseUp (const MouseEvent& e)
{
  lassoComp.endLasso();
  removeChildComponent (&lassoComp);
}

//LassoSource
void iSpaceComponent::findLassoItemsInArea (Array <Component*>& results, const Rectangle<int>& area)
{
  //  const Rectangle<int> lasso (area - subCompHolder->getPosition());
  //  
  //  for (int i = 0; i < subCompHolder->getNumChildComponents(); ++i)
  //  {
  //    Component* c = subCompHolder->getChildComponent (i);
  //    
  //    if (c->getBounds().intersects (lasso))
  //      results.add (c);
  //  }
}

SelectedItemSet <Component*>& iSpaceComponent::getLassoSelection()
{
  return selectedItems;
}
