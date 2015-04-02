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
#include "pMixConsole.h"
#include "pMixLogger.h"
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
  
  StringArray getRenderingEngines() const;
  int getActiveRenderingEngine() const;
  void setRenderingEngine (int index);
  void setOpenGLRenderingEngine();
  
private:
  pMixLookAndFeel lookAndFeel;
  PMixAudioEngine& audioEngine;
  
  ScopedPointer<GraphEditor> graphEditor;
  StretchableLayoutManager horizontalLayout;
  ScopedPointer<StretchableLayoutResizerBar> horizontalDividerBar;

  PMixLogger logger;
  ScopedPointer<PMixTabContainer> rightHandPanel;
  
#if JUCE_OPENGL
  OpenGLContext openGLContext;
#endif
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};


#endif  // GRAPHDOCUMENTCOMPONENT_H_INCLUDED
