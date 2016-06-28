/*
  ==============================================================================

    pMixCodeEditor.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#ifndef PMIXCODEEDITOR_H_INCLUDED
#define PMIXCODEEDITOR_H_INCLUDED

#include "JuceHeader.h"
#include "pMixAudioEngine.h"
#include "pMixSVGDisplay.h"
#include "pMixConsole.h"
#include "pMixGraphEditor.h"

namespace CodeEditorBottomViewIDs
{
  static const int diagram = 1;
  static const int console = 2;
};

class CodeEditorToolbarItemFactory;

class CodeEditor : public Component
                 , public ChangeListener
                 , public Button::Listener
                 , public ApplicationCommandTarget
{
public:
  CodeEditor(PMixAudioEngine& audioEngine, GraphEditor& graphEditor);
  ~CodeEditor();
  void paint (Graphics& g) override;
  
  void resized() override;
  
  void changeListenerCallback (ChangeBroadcaster* source) override;
  void buttonClicked (Button* button) override;
  
  //ApplicationCommandTarget
  ApplicationCommandTarget* getNextCommandTarget() override;
  void getAllCommands (Array <CommandID>& commands) override;
  void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result) override;
  bool perform (const InvocationInfo& info) override;
  
  void clear();
  
  void showConsoleOrBrowser(int which);
  
private:
  FaustTokeniser tokeniser;
  CodeDocument codeDocument;
  
  ScopedPointer<Toolbar> toolBar;
  
  ScopedPointer<CodeEditorComponent> editor;
  ScopedPointer<SVGDisplay> svgDisplay;
  ScopedPointer<PMixConsole> console;

  PMixAudioEngine& audioEngine;
  GraphEditor& graphEditor;

  StretchableLayoutManager verticalLayout;
  ScopedPointer<StretchableLayoutResizerBar> dividerBar1, dividerBar2;
  FaustAudioPluginInstance* selectedFaustAudioPluginInstance;
  uint32 selectedNodeId;
  int show;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CodeEditor);
};



#endif  // CODEEDITOR_H_INCLUDED
