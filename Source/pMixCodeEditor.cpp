/*
  ==============================================================================

    pMixCodeEditor.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixCodeEditor.h"
#include "pMixGraphEditor.h"
#include "pMixCommandIDs.h"

class PMixToolbarButton : public ToolbarItemComponent
{
public:
  PMixToolbarButton (const int toolbarItemId, String buttonText)
  : ToolbarItemComponent (toolbarItemId, buttonText, false)
  , button (buttonText)
  {
    addAndMakeVisible (button);
  }
  
  bool getToolbarItemSizes (int toolbarDepth, bool isVertical, int& preferredSize, int& minSize, int& maxSize) override
  {
    if (isVertical)
      return false;
    
    preferredSize = 150;
    minSize = 80;
    maxSize = 150;
    return true;
  }
  
  void paintButtonArea (Graphics&, int, int, bool, bool) override
  {
  }
  
  void contentAreaChanged (const Rectangle<int>& newArea) override
  {
    button.setSize (newArea.getWidth() - 2, jmin (newArea.getHeight() - 2, 22));
    button.setCentrePosition (newArea.getCentreX(), newArea.getCentreY());
  }
  
public:
  CompileButton button;
};


class CodeEditorToolbarItemFactory   : public ToolbarItemFactory
{
public:
  
  CodeEditorToolbarItemFactory(CodeEditor& codeEditor)
  : codeEditor(codeEditor)
  {}
  
  enum ToolbarItemIds
  {
    kCompile = 1,
    kLoad = 2,
    kSave = 3
  };
  
  void getAllToolbarItemIds (Array<int>& ids) override
  {
    ids.add (kCompile);
    ids.add (kLoad);
    ids.add (kSave);

//    ids.add (separatorBarId);
//    ids.add (spacerId);
//    ids.add (flexibleSpacerId);
  }
  
  void getDefaultItemSet (Array<int>& ids) override
  {
    ids.add (kCompile);
    ids.add (kLoad);
    ids.add (kSave);
  }
  
  ToolbarItemComponent* createItem (int itemId) override
  {
    switch (itemId)
    {
      case kCompile:
      {
        PMixToolbarButton* compileButton = new PMixToolbarButton (itemId, "Compile");
        compileButton->button.addListener(&codeEditor);
        
        codeEditor.compileButton = &compileButton->button; //TODO: this is shit
        
        return compileButton;
      }
      case kLoad:
      {
        PMixToolbarButton* compileButton = new PMixToolbarButton (itemId, "Load");
        compileButton->button.addListener(&codeEditor);
        return compileButton;
      }
      case kSave:
      {
        PMixToolbarButton* compileButton = new PMixToolbarButton (itemId, "Save");
        compileButton->button.addListener(&codeEditor);
        return compileButton;
      }
      default:
        break;
    }
    
    return nullptr;
  }
  
private:
  StringArray iconNames;
  CodeEditor& codeEditor;
};

CodeEditor::CodeEditor(PMixAudioEngine& audioEngine, GraphEditor& graphEditor)
: compileButton(nullptr)
, audioEngine(audioEngine)
, graphEditor(graphEditor)
, selectedFaustAudioPluginInstance(nullptr)
, selectedNodeId(0)
, show(CodeEditorBottomViewIDs::diagram)
{
  addAndMakeVisible (toolBar = new Toolbar());
  CodeEditorToolbarItemFactory factory(*this);
  toolBar->addDefaultItems (factory);
  
  verticalLayout.setItemLayout (0, 100, -1., -0.35);
  verticalLayout.setItemLayout (1, 8, 8, 8);
  verticalLayout.setItemLayout (2, 2, -1.0, 100);
  
  addAndMakeVisible (editor = new CodeEditorComponent (codeDocument, &tokeniser));
  //editor->setCommandManager(&getCommandManager());
  addAndMakeVisible(dividerBar1 = new StretchableLayoutResizerBar (&verticalLayout, 1, false));
  addAndMakeVisible (svgDisplay = new SVGDisplay(audioEngine, graphEditor));
  addAndMakeVisible(console = new PMixConsole());
  audioEngine.getLogger().addChangeListener(console);

  editor->setFont(Font(Font::getDefaultMonospacedFontName(), 16.f, 0));
  editor->setTabSize(2, true);
  
  graphEditor.addChangeListener(this);
  
#if PMIX_PLUGIN==0
  getCommandManager().registerAllCommandsForTarget (this);
#endif
  
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

  g.setColour (Colours::black);
  g.setFont (30.0f);
  g.drawFittedText ("Create or select a Faust node to view the code", getLocalBounds(), Justification::centred, 1);
}

void CodeEditor::resized()
{
  Rectangle<int> area (getLocalBounds());
  toolBar->setBounds (area.removeFromTop (25));

  Component* vcomps[3];//{ editor, dividerBar1, console };
  vcomps[0] = editor;
  vcomps[1] = dividerBar1;
  
  if (show == CodeEditorBottomViewIDs::diagram)
  {
    console->setVisible(false);
    svgDisplay->setVisible(true);
    vcomps[2] = svgDisplay;
  }
  else
  {
    console->setVisible(true);
    svgDisplay->setVisible(false);
    vcomps[2] = console;
  }
  
  verticalLayout.layOutComponents (vcomps, 3,
                                   area.getX(), area.getY(), area.getWidth(), area.getHeight(),
                                   true,     // lay out side-by-side
                                   true);     // resize the components' heights as well as widths
  
  repaint();
}

void CodeEditor::changeListenerCallback (ChangeBroadcaster* source)
{
  if (source == &graphEditor)
  {
    if(graphEditor.getLassoSelection().getNumSelected() == 1)
    {
      NodeComponent* selectedItem = dynamic_cast<NodeComponent*>(graphEditor.getLassoSelection().getSelectedItem(0));
      
      if (selectedItem)
      {
        selectedNodeId = selectedItem->nodeId;
        FaustAudioPluginInstance* faustProc = dynamic_cast<FaustAudioPluginInstance*>(audioEngine.getDoc().getNodeForId(selectedNodeId)->getProcessor());
        
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
            
            graphEditor.getComponentForNode(selectedNodeId)->bubbleMessage(error);
          }
          
          editor->loadContent(selectedFaustAudioPluginInstance->getSourceCode());
          editor->setVisible(true);
          toolBar->setVisible(true);
          return;
        }
      }
    }
  }
  
  clear();
}

void CodeEditor::buttonClicked (Button* button)
{
  if(button->getName() == "Compile")
  {
    String newSourceCode = codeDocument.getAllContent();
    graphEditor.updateFaustNode(selectedNodeId, newSourceCode);
    compileButton->setHighlight(false);
  }
  else  if(button->getName() == "Load")
  {
    FileChooser fc ("Load Faust DSP file...",
                    File::getSpecialLocation(File::userDesktopDirectory),
                    "*.dsp",
                    true);
    
    if (fc.browseForFileToOpen())
    {
      File chosen = fc.getResults().getLast();
    
      FileInputStream fileInput(chosen);
      
      codeDocument.loadFromStream(fileInput);
      selectedFaustAudioPluginInstance->setSourceCode(codeDocument.getAllContent(), false);
      compileButton->setHighlight(true);
    }
  }
  else if(button->getName() == "Save")
  {
    FileChooser fc ("Save Faust DSP file...",
                    File::getSpecialLocation(File::userDesktopDirectory),
                    "dsp",
                    true);
    
    if (fc.browseForFileToSave(true))
    {
      File chosen = fc.getResults().getLast();
      
      FileOutputStream fileOutput(chosen);
      
      codeDocument.writeToStream(fileOutput);
    }
  }
}

#pragma mark ApplicationCommandTarget

ApplicationCommandTarget* CodeEditor::getNextCommandTarget()
{
  return findFirstTargetParentComponent();
}

void CodeEditor::getAllCommands (Array <CommandID>& commands)
{
  // this returns the set of all commands that this target can perform..
  const CommandID ids[] = {
    CommandIDs::compile
//    CommandIDs::zoomIn ,
//    CommandIDs::zoomOut ,
//    CommandIDs::zoomNormal
  };
  
  commands.addArray (ids, numElementsInArray (ids));
}

void CodeEditor::getCommandInfo (const CommandID commandID, ApplicationCommandInfo& result)
{
  const String category (TRANS("General"));
  
  switch (commandID)
  {
    case CommandIDs::compile:
      result.setInfo (TRANS("Compile"), TRANS("Compile selected FAUST node"), category, 0);
      result.defaultKeypresses.add (KeyPress (KeyPress::returnKey, ModifierKeys::commandModifier, 0));
      break;
//    case CommandIDs::zoomIn:
//      result.setInfo (TRANS("Zoom in"), TRANS("Zooms in on the current component."), category, 0);
//      result.defaultKeypresses.add (KeyPress (']', ModifierKeys::commandModifier, 0));
//      break;
//      
//    case CommandIDs::zoomOut:
//      result.setInfo (TRANS("Zoom out"), TRANS("Zooms out on the current component."), category, 0);
//      result.defaultKeypresses.add (KeyPress ('[', ModifierKeys::commandModifier, 0));
//      break;
//      
//    case CommandIDs::zoomNormal:
//      result.setInfo (TRANS("Zoom to 100%"), TRANS("Restores the zoom level to normal."), category, 0);
//      result.defaultKeypresses.add (KeyPress ('1', ModifierKeys::commandModifier, 0));
//      break;
  }
}

bool CodeEditor::perform (const InvocationInfo& info)
{
  switch (info.commandID)
  {
    case CommandIDs::compile:
    {
      if(selectedNodeId > 0)
      {
        String newSourceCode = codeDocument.getAllContent();
        graphEditor.updateFaustNode(selectedNodeId, newSourceCode);
        compileButton->setHighlight(false);
        break;
      }
    }
    default:
      return false;
  }
  
  return true;
}
      //PopupMenu CodeEditor::getMenuForIndex (int menuIndex, const String& menuName)
//{
//  PopupMenu menu;
//
//  switch (menuIndex)
//  {
//    case CodeEditorMenuIDs::fileMenu:
//      menu.addItem(1, "Recompile", selectedFaustAudioPluginInstance != nullptr);
//      menu.addSeparator();
//      menu.addItem(2, "Import .dsp file", false);
//      menu.addItem(3, "Save As .dsp file ...", false);
//      menu.addItem(4, "Export ...", false);
//      break;
//    case CodeEditorMenuIDs::editMenu:
//      menu.addItem(1, "Copy", false);
//      menu.addItem(2, "Paste", false);
//      menu.addItem(3, "Delete", false);
//      menu.addItem(4, "Select All", false);
//      menu.addSeparator();
//      menu.addItem(5, "Comment Selection", false);
//      menu.addItem(6, "Uncomment Selection", false);
//
//      break;
//    case CodeEditorMenuIDs::viewMenu:
//      menu.addItem(CodeEditorBottomViewIDs::diagram, "Show Diagram", true, show == CodeEditorBottomViewIDs::diagram);
//      menu.addItem(CodeEditorBottomViewIDs::console, "Show Console", true, show == CodeEditorBottomViewIDs::console);
//      menu.addSeparator();
//      menu.addItem(3, "Increase Font Size", false, false);
//      menu.addItem(4, "Decrease Font Size", false, false);
//      menu.addItem(5, "Editor Settings...", false, false);
//      break;
//    case CodeEditorMenuIDs::helpMenu:
//      menu.addItem(1, "Open Faust Libraries", false);
//      menu.addItem(2, "Read PDF guide", false);
//      break;
//    default:
//      break;
//  }
//  
//  return menu;
//}

void CodeEditor::clear()
{
  selectedNodeId = 0;
  selectedFaustAudioPluginInstance = nullptr;
  editor->setVisible(false);
  toolBar->setVisible(false);
  svgDisplay->clearDisplay();
}

void CodeEditor::showConsoleOrBrowser(int which)
{
  show = which;
  resized();
}
