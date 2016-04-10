/*
  ==============================================================================

    pMixMainComponent.h
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
  
  ScopedPointer<GraphEditor> graphEditor;
  ScopedPointer<CodeEditor> codeEditor;
  ScopedPointer<InterpolationSpace> iSpace;
  
  StretchableLayoutManager horizontalLayout;
  ScopedPointer<StretchableLayoutResizerBar> horizontalDividerBar;

  ScopedPointer<PMixTabContainer> rightHandPanel;
  
  PMixFloatWindow* iSpaceWindow;
  PMixFloatWindow* codeEditorWindow;
  
#if JUCE_OPENGL
  OpenGLContext openGLContext;
#endif
  
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};


#endif  // GRAPHDOCUMENTCOMPONENT_H_INCLUDED
