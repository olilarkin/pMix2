/*
  ==============================================================================

    CodeEditor.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixCodeEditor.h"
#include "pMixGraphEditor.h"

CodeEditor::CodeEditor(PMixAudioEngine& audioEngine, GraphEditor& graphEditor)
: audioEngine(audioEngine)
, graphEditor(graphEditor)
, compileButton("Compile")
, svgButton("View SVG")
, selectedFaustAudioProcessor(nullptr)
, selectedNodeID(0)
{
  verticalLayout.setItemLayout (0, -0.2, -0.8, -0.35);
  verticalLayout.setItemLayout (1, 8, 8, 8);
  verticalLayout.setItemLayout (2, 150, -1.0, -0.65);
  verticalLayout.setItemLayout (3, 8, 8, 8);
  verticalLayout.setItemLayout (4, 150, -1.0, -0.65);
  
  addAndMakeVisible (editor = new CodeEditorComponent (codeDocument, &tokeniser));
  addAndMakeVisible(dividerBar1 = new StretchableLayoutResizerBar (&verticalLayout, 1, false));
  addAndMakeVisible (webBrowser = new WebBrowser(audioEngine));
  graphEditor.addChangeListener(webBrowser);
  addAndMakeVisible(dividerBar2 = new StretchableLayoutResizerBar (&verticalLayout, 3, false));
  addAndMakeVisible (console = new Console());
  addAndMakeVisible (&compileButton);
  addAndMakeVisible (&svgButton);

  editor->setFont(Font ("Andale Mono", 16.f, 0));
  editor->setTabSize(2, true);
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
//  editor->setBounds (r.withTrimmedTop (40));
  
  Component* vcomps[] = { editor, dividerBar1, webBrowser, dividerBar2, console };
  
  verticalLayout.layOutComponents (vcomps, 5,
                                   0, 40, getWidth(), getHeight() - 40,
                                   true,     // lay out side-by-side
                                   true);     // resize the components' heights as well as widths
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
      console->clear();
      String newSourceCode = codeDocument.getAllContent();
      PluginWindow::closeCurrentlyOpenWindowsFor (selectedNodeID);
      graphEditor.updateFaustNode(selectedNodeID, newSourceCode);
    }
    else if (button == &svgButton)
    {
      selectedFaustAudioProcessor->getFactory()->display_svg();
    }
  }
}
