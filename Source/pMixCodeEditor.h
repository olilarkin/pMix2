/*
  ==============================================================================

    pMixCodeEditor.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#pragma once

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

class CompileButton : public TextButton
{
public:
  CompileButton(String buttonName)
  : TextButton(buttonName)
  , highlight(false)
  {
  }
  
  ~CompileButton()
  {
  }
  
  void paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown)
  {
    LookAndFeel& lf = getLookAndFeel();
    
    Colour bgColour;
    
    if(highlight)
      bgColour = getToggleState() ? findColour(buttonOnColourId) : Colours::red;
    else
      bgColour = findColour (getToggleState() ? buttonOnColourId : buttonColourId);
    
    lf.drawButtonBackground (g, *this,bgColour, isMouseOverButton, isButtonDown);
    
    lf.drawButtonText (g, *this, isMouseOverButton, isButtonDown);
  }
  
  void setHighlight(bool v)
  {
    highlight = v;
    repaint();
  }

private:
  bool highlight;
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
  
  CompileButton* compileButton;
  
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
  File lastDirectory;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CodeEditor);
};



