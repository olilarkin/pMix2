/*
  ==============================================================================

    MainComponent.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "FilterGraph.h"
#include "LookAndFeel.h"
#include "InterpolationSpace.h"
#include "GraphEditorPanel.h"

class MainComponent  : public Component
{
public:
  
  MainComponent (AudioPluginFormatManager& formatManager, AudioDeviceManager* deviceManager);
  ~MainComponent();
  
  void createNewPlugin (const PluginDescription* desc, int x, int y);
  void resized();
  void setZoom (double scale);
  double getZoom() const;
  
  FilterGraph graph;
  UndoManager undoManager;
  
private:
  
  AudioDeviceManager* deviceManager;
  AudioProcessorPlayer graphPlayer;
  MidiKeyboardState keyState;
  
  GraphEditorPanel* graphPanel;
  Component* keyboardComp;
  Component* statusBar;
  
  pMixLookAndFeel lf;
  iSpaceComponent* iSpace;
  ParamView* paramView;
  
  StretchableLayoutManager verticalLayout;
  StretchableLayoutResizerBar* verticalDividerBar;
  
  //  ParamTreeView* treeView;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};


#endif  // GRAPHDOCUMENTCOMPONENT_H_INCLUDED
