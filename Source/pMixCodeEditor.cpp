/*
  ==============================================================================

    CodeEditor.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixCodeEditor.h"
#include "pMixGraphEditor.h"
#include "FaustAudioProcessor.h"

CodeEditor::CodeEditor(PMixAudioEngine& audioEngine)
: audioEngine(audioEngine)
, compileButton("Compile")
{
  addAndMakeVisible (editor = new CodeEditorComponent (codeDocument, &cppTokeniser));
  addAndMakeVisible (&compileButton);
  editor->loadContent ("");
  
  compileButton.addListener(this);
}

void CodeEditor::paint (Graphics& g)
{
  //g.fillAll (Colours::lightgrey);
}

void CodeEditor::resized()
{
  Rectangle<int> r (getLocalBounds().reduced (8));
  compileButton.setBounds(8, 8, 100, 20 );
  editor->setBounds (r.withTrimmedTop (40));
}

void CodeEditor::changeListenerCallback (ChangeBroadcaster* source)
{
  GraphEditor* graphEditor = dynamic_cast<GraphEditor*>(source);
  
  if (graphEditor)
  {
    if(graphEditor->getLassoSelection().getNumSelected() == 1)
    {
      FilterComponent* selectedItem = dynamic_cast<FilterComponent*>(graphEditor->getLassoSelection().getSelectedItem(0));
      
      if (selectedItem)
      {
        FaustAudioProcessor* faustProc = dynamic_cast<FaustAudioProcessor*>(audioEngine.getDoc().getNodeForId(selectedItem->filterID)->getProcessor());
        
        if (faustProc)
        {
          editor->loadContent(faustProc->get_sourcecode());
        }
      }
    }
  }
}

void CodeEditor::buttonClicked (Button* button)
{
  if (button == &compileButton)
    editor->loadContent("compiled");
}
