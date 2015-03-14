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
//#include "pMixFileBrowser.h"
#include "pMixConsole.h"
#include "pMixLogger.h"
//#include "pMixParamTreeView.h"
//#include "pMixParamView.h"

class MainComponent  : public Component
{
public:
  MainComponent (PMixAudioEngine& audioEngine);
  ~MainComponent();
  
  void resized();
  void setZoom (double scale);
  double getZoom() const;

private:
  pMixLookAndFeel lookAndFeel;
  PMixAudioEngine& audioEngine;
  
  ScopedPointer<CodeEditor> codeEditor;
  ScopedPointer<GraphEditor> graphEditor;
//  ScopedPointer<FileBrowser> fileBrowser;
  ScopedPointer<InterpolationSpace> interpolationSpace;
  StretchableLayoutManager horizontalLayout;
  ScopedPointer<StretchableLayoutResizerBar> horizontalDividerBar;
//  ScopedPointer<ParamTreeView> paramTreeView;
//  ScopedPointer<ParamView> paramView;

  PMixLogger logger;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};


#endif  // GRAPHDOCUMENTCOMPONENT_H_INCLUDED
