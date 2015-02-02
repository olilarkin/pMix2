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
#include "SplitComponent.h"
#include "pMixFileBrowser.h"

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
  //MidiKeyboardState keyState;
  
  ScopedPointer<SplitComponent> splitComponent;
  ScopedPointer<CodeEditor> codeEditor;
  ScopedPointer<GraphEditor> graphEditor;
  ScopedPointer<TooltipBar> statusBar;
  ScopedPointer<FileBrowser> fileBrowser;
  ScopedPointer<WebBrowserComponent> webBrowser;
  ScopedPointer<InterpolationSpaceComponent> interpolationSpace;
  ScopedPointer<ParamView> paramView;
  StretchableLayoutManager verticalLayout;
  ScopedPointer<StretchableLayoutResizerBar> verticalDividerBar;
  
  //  ParamTreeView* treeView;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};


#endif  // GRAPHDOCUMENTCOMPONENT_H_INCLUDED
