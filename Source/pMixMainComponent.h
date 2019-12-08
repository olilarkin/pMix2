/*
  ==============================================================================

    pMixMainComponent.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#pragma once

#include "pMixAudioEngine.h"
#include "pMixInterpolationSpace.h"
#include "pMixGraphEditor.h"
#include "pMixCodeEditor.h"
#include "pMixLookAndFeel.h"
#include "pMixRHPTabContainer.h"
#include "pMixFloatWindow.h"

class MainComponent  : public Component
{
public:
  enum EFloatWindows
  {
    kWindowCodeEditor = 0,
    kWindowISpace,
    kWindowConsole
  };
  
  MainComponent (PMixAudioEngine& audioEngine);
  ~MainComponent();
  
  void paint (Graphics& g);
  void resized();
  void setZoom (double scale);
  double getZoom() const;
  void clear();
  
  void floatWindow(int whichWindow, bool floatIt);
  
  StringArray getRenderingEngines() const;
  int getActiveRenderingEngine() const;
  void setRenderingEngine (int index);
  void setOpenGLRenderingEngine();
  
private:
  pMixLookAndFeel lookAndFeel;
  PMixAudioEngine& audioEngine;
  
  std::unique_ptr<GraphEditor> graphEditor;
  std::unique_ptr<CodeEditor> codeEditor;
  std::unique_ptr<InterpolationSpace> iSpace;
  
  StretchableLayoutManager horizontalLayout;
  std::unique_ptr<StretchableLayoutResizerBar> horizontalDividerBar;

  std::unique_ptr<PMixTabContainer> rightHandPanel;
  
  PMixFloatWindow* iSpaceWindow;
  PMixFloatWindow* codeEditorWindow;
  
#if JUCE_OPENGL
  OpenGLContext openGLContext;
#endif
  
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};


