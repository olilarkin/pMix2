/*
  ==============================================================================

    CodeEditor.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixCodeEditor.h"
#include "pMixGraphEditor.h"

CodeEditor::CodeEditor(PMixAudioEngine& audioEngine, WebBrowser& webBrowser, Console& console)
: audioEngine(audioEngine)
, webBrowser(webBrowser)
, console(console)
, compileButton("Compile")
, svgButton("View SVG")
, selectedFaustAudioProcessor(nullptr)
{
  addAndMakeVisible (editor = new CodeEditorComponent (codeDocument, &cppTokeniser));
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
      console.clear();
      selectedFaustAudioProcessor->getFactory()->update_sourcecode(codeDocument.getAllContent(), selectedFaustAudioProcessor);
      webBrowser.browser->goToURL(selectedFaustAudioProcessor->getFactory()->get_svg_path());
    }
    else if (button == &svgButton)
    {
      selectedFaustAudioProcessor->getFactory()->display_svg();
    }
  }
}
