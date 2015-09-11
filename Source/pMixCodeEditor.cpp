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
, selectedFaustAudioPluginInstance(nullptr)
, selectedNodeID(0)
{
  addAndMakeVisible (menuBar = new MenuBarComponent (this));

  verticalLayout.setItemLayout (0, 100, -1., -0.35);
  verticalLayout.setItemLayout (1, 8, 8, 8);
  verticalLayout.setItemLayout (2, 2, -1.0, 100);
  
  addAndMakeVisible (editor = new CodeEditorComponent (codeDocument, &tokeniser));
  addAndMakeVisible(dividerBar1 = new StretchableLayoutResizerBar (&verticalLayout, 1, false));
  addAndMakeVisible (webBrowser = new WebBrowser(audioEngine, graphEditor));
  addAndMakeVisible(console = new Console());
  audioEngine.getLogger().addChangeListener(console);

  editor->setFont(Font(Font::getDefaultMonospacedFontName(), 16.f, 0));
  editor->setTabSize(2, true);
  editor->loadContent ("");
  
  graphEditor.addChangeListener(this);
  
  audioEngine.getLogger().logMessage("Ready.");
}

CodeEditor::~CodeEditor()
{
  graphEditor.removeChangeListener(this);
  audioEngine.getLogger().removeChangeListener(console);
}

void CodeEditor::paint (Graphics& g)
{
  //g.fillAll (Colours::lightgrey);
}

void CodeEditor::resized()
{
  Rectangle<int> area (getLocalBounds());
  menuBar->setBounds (area.removeFromTop (LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight()));

  Component* vcomps[] = { editor, dividerBar1, webBrowser };
  
  verticalLayout.layOutComponents (vcomps, 3,
                                   area.getX(), area.getY(), area.getWidth(), area.getHeight(),
                                   true,     // lay out side-by-side
                                   true);     // resize the components' heights as well as widths
}

void CodeEditor::changeListenerCallback (ChangeBroadcaster* source)
{
  if (source == &graphEditor)
  {
//    if (selectedFaustAudioPluginInstance != nullptr)
//    {
//      selectedFaustAudioPluginInstance->setSourceCode(codeDocument.getAllContent(), false);
//    }
    
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
          
          // todo if instance is compiled OK
          selectedFaustAudioPluginInstance->getFactory()->startSVGThread();  
                  
          editor->loadContent(selectedFaustAudioPluginInstance->getSourceCode());
          editor->setInterceptsMouseClicks(true, true);
          return;
        }
      }
    }
  }
  
  clear();
}


StringArray CodeEditor::getMenuBarNames()
{
  const char* const names[] = { "File", "Edit", "View", "Help", nullptr };
  
  return StringArray (names);
}

PopupMenu CodeEditor::getMenuForIndex (int menuIndex, const String& menuName)
{
  PopupMenu menu;

  switch (menuIndex)
  {
    case 0:
      menu.addItem(1, "Recompile", selectedFaustAudioPluginInstance != nullptr);
      menu.addSeparator();
      menu.addItem(2, "Import .dsp file", false);
      menu.addItem(3, "Save As .dsp file ...", false);
      menu.addItem(4, "Export ...", false);
      break;
    case 1:
      menu.addItem(1, "Copy", false);
      menu.addItem(2, "Paste", false);
      menu.addItem(3, "Delete", false);
      menu.addItem(4, "Select All", false);
      break;
    case 2:
      menu.addItem(1, "Show Diagram", true, true);
      menu.addItem(2, "Show Console", true, false);
      menu.addSeparator();
      menu.addItem(3, "Increase Font Size", false, false);
      menu.addItem(4, "Decrease Font Size", false, false);
      menu.addItem(5, "Editor Settings...", false, false);
      break;
    case 3:
      menu.addItem(1, "Open Faust Libraries", false);
      menu.addItem(2, "Read PDF guide", false);
      break;
    default:
      break;
  }
  
  return menu;
}

void CodeEditor::menuItemSelected (int menuItemID, int topLevelMenuIndex)
{
  switch (topLevelMenuIndex)
  {
    case 0:
    {
      switch (menuItemID)
      {
        case 1:
        {
          String newSourceCode = codeDocument.getAllContent();
          graphEditor.updateFaustNode(selectedNodeID, newSourceCode);
          break;
        }
      }
      break;
    }
    default:
      break;
  }
}

void CodeEditor::clear()
{
  selectedFaustAudioPluginInstance = nullptr;
  editor->loadContent("Create or select a Faust filter to view the code");
  editor->setInterceptsMouseClicks(false, false);
  webBrowser->browser->goToURL("");
}
