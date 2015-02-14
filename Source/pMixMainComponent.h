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
#include "pMixWebBrowser.h"
#include "pMixLookAndFeel.h"
#include "SplitComponent.h"
#include "pMixFileBrowser.h"
#include "pMixConsole.h"
#include "pMixLogger.h"

class MainComponent  : public Component
{
public:
  MainComponent (PMixAudioEngine& audioEngine);
  ~MainComponent();
  
  void createNewPlugin (const PluginDescription* desc, int x, int y);
  void resized();
  void setZoom (double scale);
  double getZoom() const;

private:
  pMixLookAndFeel lookAndFeel;
  PMixAudioEngine& audioEngine;  
  
  ScopedPointer<SplitComponent> vsplitComponent;
  ScopedPointer<SplitComponent> hsplitComponent;

  ScopedPointer<Console> console;
  ScopedPointer<CodeEditor> codeEditor;
  ScopedPointer<GraphEditor> graphEditor;
  ScopedPointer<FileBrowser> fileBrowser;
  ScopedPointer<WebBrowser> webBrowser;
  ScopedPointer<InterpolationSpaceComponent> interpolationSpace;
  ScopedPointer<ParamView> paramView;
  StretchableLayoutManager verticalLayout;
  ScopedPointer<StretchableLayoutResizerBar> verticalDividerBar;
  //  ParamTreeView* treeView;
  
  PMixLogger logger;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};


#endif  // GRAPHDOCUMENTCOMPONENT_H_INCLUDED
