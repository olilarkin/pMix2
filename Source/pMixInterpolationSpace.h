
#ifndef _INTERPOLATIONSPACE_H_
#define _INTERPOLATIONSPACE_H_

#include "JuceHeader.h"
#include "pMixAudioEngine.h"
#include "pMixInterpolationSpaceActions.h"

class InterpolationSpaceLabel : public Label
{
public:
  InterpolationSpaceLabel(const String& labelText);
};

class InterpolationSpacePreset : public Component
{
private:
  ComponentDragger myDragger;
  ComponentBoundsConstrainer boundsConstrainer;
  Rectangle<int> startBounds;
  Rectangle<int> endBounds;
  ScopedPointer<InterpolationSpaceLabel> label;
  
public:
  PMixAudioEngine& audioEngine;
  const uint32 filterID;
  const uint32 presetIdx;
  Colour colour;
public:
  InterpolationSpacePreset(PMixAudioEngine& audioEngine, String& initalLabel, const uint32 filterID, const uint32 presetIdx, Colour colour);
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
  PMixAudioEngine& audioEngine;
  SelectedItemSet<Component*> selectedItems;
  LassoComponent<Component*> lassoComp;
  void updateComponents();

public:
  InterpolationSpaceComponent (PMixAudioEngine& audioEngine);
  ~InterpolationSpaceComponent ();
  void resized ();
  void paint (Graphics& g);
  void mouseDown (const MouseEvent& e);
  void mouseDrag (const MouseEvent& e);
  void mouseUp (const MouseEvent& e);
  void findLassoItemsInArea (Array <Component*>& results, const Rectangle<int>& area);
  SelectedItemSet <Component*>& getLassoSelection();
  
  void changeListenerCallback (ChangeBroadcaster* source);
};

#endif //_INTERPOLATIONSPACE_H_ 