
#ifndef _INTERPOLATIONSPACE_H_
#define _INTERPOLATIONSPACE_H_

#include "../JuceLibraryCode/JuceHeader.h"
#include "pMixAudio.h"

class InterpolationSpaceLabel : public Label
{
public:
  InterpolationSpaceLabel(const String& labelText);
};

class MovePresetAction : public UndoableAction
{
public:
  MovePresetAction (Component* interpolationSpace, const String& componentID, Rectangle<int> startBounds, Rectangle<int> endBounds) noexcept;
  bool perform();
  bool undo();
  int getSizeInUnits();
  
private:
  Component* interpolationSpace;
  String componentID;
  Rectangle<int> startBounds;
  Rectangle<int> endBounds;
  JUCE_DECLARE_NON_COPYABLE (MovePresetAction)
};

class InterpolationSpacePreset : public Component
{
private:
  ComponentDragger myDragger;
  ComponentBoundsConstrainer boundsConstrainer;
  PMixAudio& audio;
  Rectangle<int> startBounds;
  Rectangle<int> endBounds;
  InterpolationSpaceLabel* label;
    
public:
  InterpolationSpacePreset(PMixAudio& audio, String& initalLabel);
  ~InterpolationSpacePreset ();
  void resized ();
  void mouseDown (const MouseEvent& e);
  void mouseDrag (const MouseEvent& e);
  void mouseUp (const MouseEvent& e);
  void paint (Graphics& g);
};

class InterpolationSpaceComponent  : public Component
                                   , public LassoSource<Component*>
                                   , public ChangeListener
{
private:
  //TooltipWindow tooltipWindow;
  PMixAudio& audio;
  Random mRand;
  SelectedItemSet<Component*> selectedItems;
  LassoComponent<Component*> lassoComp;
  
public:
  InterpolationSpaceComponent (PMixAudio& audio);
  ~InterpolationSpaceComponent ();
  void resized ();
  void paint (Graphics& g);
  void mouseDown (const MouseEvent& e);
  void mouseDrag (const MouseEvent& e);
  void mouseUp (const MouseEvent& e);
  void findLassoItemsInArea (Array <Component*>& results, const Rectangle<int>& area);
  SelectedItemSet <Component*>& getLassoSelection();
  
  void changeListenerCallback (ChangeBroadcaster*);
};

#endif //_INTERPOLATIONSPACE_H_ 