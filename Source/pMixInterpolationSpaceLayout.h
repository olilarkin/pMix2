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
#include "pMixGraphEditor.h"

class InterpolationSpaceLabel : public Label
{
public:
  InterpolationSpaceLabel(const String& labelText);
};

class InterpolationSpacePreset : public Component
                               , public ChangeListener
                               , public LabelListener
{
public:
  InterpolationSpacePreset(PMixAudioEngine& audioEngine, String& initalLabel, const uint32 nodeId, const uint32 presetIdx, Colour colour);
  ~InterpolationSpacePreset ();
  void resized ();
  void mouseDown (const MouseEvent& e);
  void mouseDrag (const MouseEvent& e);
  void mouseUp (const MouseEvent& e);
  void paint (Graphics& g);
  
  void changeListenerCallback (ChangeBroadcaster* source);

  void labelTextChanged (Label* labelThatHasChanged);

  
  void update();
  
public:
  PMixAudioEngine& audioEngine;
  const uint32 nodeId;
  const uint32 presetIdx;
  
private:
  ComponentDragger myDragger;
  ComponentBoundsConstrainer boundsConstrainer;
  Rectangle<int> startBounds;
  Rectangle<int> endBounds;
  ScopedPointer<InterpolationSpaceLabel> label;
  Colour colour;
  float opacity;
};

class PMixInterpolationSpaceLayout : public Component
                                   , public LassoSource<Component*>
                                   , public ChangeListener
{
public:
  PMixInterpolationSpaceLayout(PMixAudioEngine& audioEngine, GraphEditor& graphEditor);
  ~PMixInterpolationSpaceLayout();

  void paint (Graphics&);
  void resized();
  
  void mouseDown (const MouseEvent& e);
  void mouseDrag (const MouseEvent& e);
  void mouseUp (const MouseEvent& e);
  
  void findLassoItemsInArea (Array <Component*>& results, const Rectangle<int>& area);
  SelectedItemSet <Component*>& getLassoSelection();
  
  void changeListenerCallback (ChangeBroadcaster* source);

  void getComponentsForFilter (const uint32 nodeId, Array<InterpolationSpacePreset*>& components) const;

  void repaintPresetsForFilter (const uint32 nodeId);
  
private:
  //TooltipWindow tooltipWindow;
  PMixAudioEngine& audioEngine;
  GraphEditor& graphEditor;
  SelectedItemSet<Component*> selectedItems;
  LassoComponent<Component*> lassoComp;
  void updateComponents();

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PMixInterpolationSpaceLayout)
};


#endif  // PMIXINTERPOLATIONSPACELAYOUT_H_INCLUDED
