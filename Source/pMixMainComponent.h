/*
  ==============================================================================

    MainComponent.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "pMixAudio.h"
#include "pMixInterpolationSpace.h"
#include "pMixGraphEditor.h"
#include "pMixCodeEditor.h"

class MainComponent  : public Component
{
public:
  MainComponent (PMixAudio& audio, AudioDeviceManager* deviceManager);
  MainComponent (PMixAudio& audio);
  ~MainComponent();
  
  void createNewPlugin (const PluginDescription* desc, int x, int y);
  void resized();
  void setZoom (double scale);
  double getZoom() const;
  //PMixDocument& getDoc() { return doc; }
private:
  PMixAudio& audio;
  
  AudioDeviceManager* deviceManager;
//  AudioProcessorPlayer& graphPlayer;
  MidiKeyboardState keyState;
  
  CodeEditor* codeEditor;
  GraphEditor* graphEditor;
  Component* keyboardComp;
  Component* statusBar;
  InterpolationSpaceComponent* interpolationSpace;
  ParamView* paramView;
  StretchableLayoutManager verticalLayout;
  StretchableLayoutResizerBar* verticalDividerBar;
  
  //  ParamTreeView* treeView;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};


#endif  // GRAPHDOCUMENTCOMPONENT_H_INCLUDED
