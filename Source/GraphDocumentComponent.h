/*
  ==============================================================================

    GraphDocumentComponent.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#ifndef GRAPHDOCUMENTCOMPONENT_H_INCLUDED
#define GRAPHDOCUMENTCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "FilterGraph.h"
#include "LookAndFeel.h"
#include "iSpace.h"
#include "GraphEditorPanel.h"

class GraphDocumentComponent  : public Component
{
public:
  
  GraphDocumentComponent (AudioPluginFormatManager& formatManager, AudioDeviceManager* deviceManager);
  ~GraphDocumentComponent();
  
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
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphDocumentComponent)
};


#endif  // GRAPHDOCUMENTCOMPONENT_H_INCLUDED
