/*
  ==============================================================================

    CodeEditor.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixCodeEditor.h"

CodeEditor::CodeEditor()
{
  addAndMakeVisible (editor = new CodeEditorComponent (codeDocument, &cppTokeniser));
  
  editor->loadContent ("\n"
                       "/* Code editor demo!\n"
                       "\n"
                       "   To see a real-world example of the code editor\n"
                       "   in action, try the Introjucer!\n"
                       "\n"
                       "*/\n"
                       "\n");

}

void CodeEditor::paint (Graphics& g)
{
  //g.fillAll (Colours::lightgrey);
}

void CodeEditor::resized()
{
  Rectangle<int> r (getLocalBounds().reduced (8));
  
  editor->setBounds (r.withTrimmedTop (8));
}