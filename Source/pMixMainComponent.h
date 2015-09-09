/*
  ==============================================================================

    MainComponent.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "pMixAudioEngine.h"
#include "pMixInterpolationSpace.h"
#include "pMixGraphEditor.h"
#include "pMixCodeEditor.h"
#include "pMixLookAndFeel.h"
#include "pMixRHPTabContainer.h"

class MainComponent  : public Component
{
public:
  MainComponent (PMixAudioEngine& audioEngine);
  ~MainComponent();
  
  void resized();
  void setZoom (double scale);
  double getZoom() const;
  void clear();
  
private:
  pMixLookAndFeel lookAndFeel;
  PMixAudioEngine& audioEngine;
  
  ScopedPointer<GraphEditor> graphEditor;
  StretchableLayoutManager horizontalLayout;
  ScopedPointer<StretchableLayoutResizerBar> horizontalDividerBar;

  ScopedPointer<PMixTabContainer> rightHandPanel;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};


#endif  // GRAPHDOCUMENTCOMPONENT_H_INCLUDED
