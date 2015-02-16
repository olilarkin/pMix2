/*
  ==============================================================================

    CodeEditor.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixCodeEditor.h"
#include "pMixGraphEditor.h"

CodeEditor::CodeEditor(PMixAudioEngine& audioEngine, WebBrowser& webBrowser, Console& console, GraphEditor& graphEditor)
: audioEngine(audioEngine)
, webBrowser(webBrowser)
, console(console)
, graphEditor(graphEditor)
, compileButton("Compile")
, svgButton("View SVG")
, selectedFaustAudioProcessor(nullptr)
, selectedNodeID(0)
{
  addAndMakeVisible (editor = new CodeEditorComponent (codeDocument, &tokeniser));
  addAndMakeVisible (&compileButton);
  addAndMakeVisible (&svgButton);

  editor->loadContent ("");
  
  compileButton.addListener(this);
  svgButton.addListener(this);
}

void CodeEditor::paint (Graphics& g)
{
  //g.fillAll (Colours::lightgrey);
}

void CodeEditor::resized()
{
  Rectangle<int> r (getLocalBounds());
  compileButton.setBounds(8, 8, 100, 20 );
  svgButton.setBounds(108, 8, 100, 20 );
  editor->setBounds (r.withTrimmedTop (40));
}

void CodeEditor::changeListenerCallback (ChangeBroadcaster* source)
{
  if (source == &graphEditor)
  {
    if(graphEditor.getLassoSelection().getNumSelected() == 1)
    {
      FilterComponent* selectedItem = dynamic_cast<FilterComponent*>(graphEditor.getLassoSelection().getSelectedItem(0));
      
      if (selectedItem)
      {
        selectedNodeID = selectedItem->filterID;
        FaustAudioProcessor* faustProc = dynamic_cast<FaustAudioProcessor*>(audioEngine.getDoc().getNodeForId(selectedNodeID)->getProcessor());
        
        if (faustProc)
        {
          selectedFaustAudioProcessor = faustProc;
          editor->loadContent(faustProc->get_sourcecode());
          return;
        }
      }
    }
  }
  
  selectedFaustAudioProcessor = nullptr;
  editor->loadContent("");
}

void CodeEditor::buttonClicked (Button* button)
{
  if (selectedFaustAudioProcessor != nullptr)
  {
    if (button == &compileButton)
    {
      //console.clear();
      String newSourceCode = codeDocument.getAllContent();
      graphEditor.updateFaustNode(selectedNodeID, newSourceCode);
    }
    else if (button == &svgButton)
    {
      selectedFaustAudioProcessor->getFactory()->display_svg();
    }
  }
}
