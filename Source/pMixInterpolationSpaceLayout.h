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
  InterpolationSpacePreset(PMixAudioEngine& audioEngine, String& initalLabel, const uint32 nodeId, const int presetId, Colour colour);
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
  const int presetId;
  
private:
  ComponentDragger myDragger;
  ComponentBoundsConstrainer boundsConstrainer;
  Rectangle<int> startBounds, endBounds;
  ScopedPointer<InterpolationSpaceLabel> label;
  Colour colour;
  float opacity;
  bool dragging;
};

class PMixInterpolationSpaceLayout : public Component
                                   , public LassoSource<Component*>
                                   , public ChangeListener
                                   , public ApplicationCommandTarget
{
public:
  PMixInterpolationSpaceLayout(PMixAudioEngine& audioEngine, GraphEditor& graphEditor);
  ~PMixInterpolationSpaceLayout();

  void paint (Graphics&) override;
  void resized () override;
  
  void mouseDown (const MouseEvent& e) override;
  void mouseDrag (const MouseEvent& e) override;
  void mouseUp (const MouseEvent& e) override;
  void mouseDoubleClick (const MouseEvent& e) override;

  void findLassoItemsInArea (Array <Component*>& results, const Rectangle<int>& area) override;
  SelectedItemSet <Component*>& getLassoSelection() override;
  
  //ApplicationCommandTarget
  ApplicationCommandTarget* getNextCommandTarget() override;
  void getAllCommands (Array <CommandID>& commands) override;
  void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result) override;
  bool perform (const InvocationInfo& info) override;
  
  void changeListenerCallback (ChangeBroadcaster* source) override;

  void getComponentsForNode (const uint32 nodeId, Array<InterpolationSpacePreset*>& components) const;

  void repaintPresetsForNode (const uint32 nodeId);
  
  void deleteSelection();
  void selectAll();
  
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
