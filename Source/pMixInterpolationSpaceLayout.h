/*
  ==============================================================================

    pMixInterpolationSpaceLayout.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#ifndef PMIXINTERPOLATIONSPACELAYOUT_H_INCLUDED
#define PMIXINTERPOLATIONSPACELAYOUT_H_INCLUDED

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

class PMixInterpolationSpaceLayout : public Component
                                   , public LassoSource<Component*>
                                   , public ChangeListener
{
public:
  PMixInterpolationSpaceLayout(PMixAudioEngine& audioEngine);
  ~PMixInterpolationSpaceLayout();

  void paint (Graphics&);
  void resized();
  
  void mouseDown (const MouseEvent& e);
  void mouseDrag (const MouseEvent& e);
  void mouseUp (const MouseEvent& e);
  
  void findLassoItemsInArea (Array <Component*>& results, const Rectangle<int>& area);
  SelectedItemSet <Component*>& getLassoSelection();
  
  void changeListenerCallback (ChangeBroadcaster* source);

private:
  //TooltipWindow tooltipWindow;
  PMixAudioEngine& audioEngine;
  SelectedItemSet<Component*> selectedItems;
  LassoComponent<Component*> lassoComp;
  void updateComponents();

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PMixInterpolationSpaceLayout)
};


#endif  // PMIXINTERPOLATIONSPACELAYOUT_H_INCLUDED
