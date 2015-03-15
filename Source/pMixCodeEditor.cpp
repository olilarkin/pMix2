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
, selectedFaustAudioPluginInstance(nullptr)
, selectedNodeID(0)
{
  verticalLayout.setItemLayout (0, 100, -1., -0.35);
  verticalLayout.setItemLayout (1, 8, 8, 8);
  verticalLayout.setItemLayout (2, 2, -1.0, 100);
  verticalLayout.setItemLayout (3, 8, 8, 8);
  verticalLayout.setItemLayout (4, 2, -1.0, -0.65);
  
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
  
  Component* vcomps[] = { editor, dividerBar1, console , dividerBar2, webBrowser };
  
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
        selectedNodeID = selectedItem->nodeId;
        FaustAudioPluginInstance* faustProc = dynamic_cast<FaustAudioPluginInstance*>(audioEngine.getDoc().getNodeForId(selectedNodeID)->getProcessor());
        
        if (faustProc)
        {
          selectedFaustAudioPluginInstance = faustProc;
          editor->loadContent(faustProc->getSourcecode());
          return;
        }
      }
    }
  }
  
  selectedFaustAudioPluginInstance = nullptr;
  editor->loadContent("");
}

void CodeEditor::buttonClicked (Button* button)
{
  if (selectedFaustAudioPluginInstance != nullptr)
  {
    if (button == &compileButton)
    {
      console->clear();
      String newSourceCode = codeDocument.getAllContent();
      graphEditor.updateFaustNode(selectedNodeID, newSourceCode);
    }
    else if (button == &svgButton)
    {
      selectedFaustAudioPluginInstance->getFactory()->displaySVG();
    }
  }
}
