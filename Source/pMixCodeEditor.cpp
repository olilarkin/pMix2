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
, show(CodeEditorBottomViewIDs::diagram)
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
  
  graphEditor.addChangeListener(this);
  
  audioEngine.getLogger().logMessage("Ready.");
  
  clear();
}

CodeEditor::~CodeEditor()
{
  graphEditor.removeChangeListener(this);
  audioEngine.getLogger().removeChangeListener(console);
}

void CodeEditor::paint (Graphics& g)
{
  g.fillAll (Colours::white);
}

void CodeEditor::resized()
{
  Rectangle<int> area (getLocalBounds());
  menuBar->setBounds (area.removeFromTop (LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight()));

  Component* vcomps[3];//{ editor, dividerBar1, console };
  vcomps[0] = editor;
  vcomps[1] = dividerBar1;
  
  if (show == CodeEditorBottomViewIDs::diagram)
  {
    console->setVisible(false);
    webBrowser->setVisible(true);
    vcomps[2] = webBrowser;
  }
  else
  {
    console->setVisible(true);
    webBrowser->setVisible(false);
    vcomps[2] = console;
  }
  
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
          
          if (!selectedFaustAudioPluginInstance->getHighlight())
          {
            selectedFaustAudioPluginInstance->getFactory()->startSVGThread();
          }
          else
          {
            String error = selectedFaustAudioPluginInstance->getCompilerMessage();
            
            String lineNo = error.fromFirstOccurrenceOf(": ", false, true).upToFirstOccurrenceOf(" :", false, true);
            
            graphEditor.getComponentForFilter(selectedNodeID)->bubbleMessage(error);
            
            //editor->setHighlightedRegion(<#const Range<int> &newRange#>);
          }
          
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
    case CodeEditorMenuIDs::fileMenu:
      menu.addItem(1, "Recompile", selectedFaustAudioPluginInstance != nullptr);
      menu.addSeparator();
      menu.addItem(2, "Import .dsp file", false);
      menu.addItem(3, "Save As .dsp file ...", false);
      menu.addItem(4, "Export ...", false);
      break;
    case CodeEditorMenuIDs::editMenu:
      menu.addItem(1, "Copy", false);
      menu.addItem(2, "Paste", false);
      menu.addItem(3, "Delete", false);
      menu.addItem(4, "Select All", false);
      menu.addSeparator();
      menu.addItem(5, "Comment Selection", false);
      menu.addItem(6, "Uncomment Selection", false);

      break;
    case CodeEditorMenuIDs::viewMenu:
      menu.addItem(CodeEditorBottomViewIDs::diagram, "Show Diagram", true, show == CodeEditorBottomViewIDs::diagram);
      menu.addItem(CodeEditorBottomViewIDs::console, "Show Console", true, show == CodeEditorBottomViewIDs::console);
      menu.addSeparator();
      menu.addItem(3, "Increase Font Size", false, false);
      menu.addItem(4, "Decrease Font Size", false, false);
      menu.addItem(5, "Editor Settings...", false, false);
      break;
    case CodeEditorMenuIDs::helpMenu:
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
    case CodeEditorMenuIDs::fileMenu:
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
    case CodeEditorMenuIDs::viewMenu:
      switch (menuItemID)
      {
        case CodeEditorBottomViewIDs::console:
          showConsoleOrBrowser(CodeEditorBottomViewIDs::console);
          break;
        case CodeEditorBottomViewIDs::diagram:
          showConsoleOrBrowser(CodeEditorBottomViewIDs::diagram);
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}

void CodeEditor::clear()
{
  selectedFaustAudioPluginInstance = nullptr;
  editor->loadContent("Create or select a Faust node to view the code");
  editor->setInterceptsMouseClicks(false, false);
  webBrowser->browser->goToURL("");
}

void CodeEditor::showConsoleOrBrowser(int which)
{
  show = which;
  resized();
}
