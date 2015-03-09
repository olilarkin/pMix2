#include "pMixMainAppWindow.h"
#include "pMixCommandIDs.h"

static double snapToIntegerZoom (double zoom)
{
  if (zoom >= 1.0)
    return (double) (int) (zoom + 0.5);
  
  return 1.0 / (int) (1.0 / zoom + 0.5);
}

MainAppWindow::MainAppWindow(PMixAudioEngine& audioEngine)
  : DocumentWindow (JUCEApplication::getInstance()->getApplicationName(), Colours::lightgrey, DocumentWindow::allButtons)
  , audioEngine(audioEngine)
{

  setUsingNativeTitleBar (true);
  setResizable (true, false);
  setResizeLimits (500, 400, 10000, 10000);
  centreWithSize (800, 600);

  setContentOwned (new MainComponent (audioEngine), false);

  restoreWindowStateFromString (audioEngine.getAppProperties().getUserSettings()->getValue ("mainWindowPos"));

  setVisible (true);

  addKeyListener (getCommandManager().getKeyMappings());

  Process::setPriority (Process::HighPriority);
  
  getCommandManager().registerAllCommandsForTarget (this);
  
#if JUCE_MAC
  
  PopupMenu pop;
  pop.addCommandItem (&getCommandManager(), CommandIDs::aboutBox);
  pop.addSeparator();
  pop.addCommandItem (&getCommandManager(), CommandIDs::showPrefs);
  pop.addSeparator();
  
  setMacMainMenu (this, &pop);
#else
  setMenuBar (this);
#endif
  
//  getCommandManager().setFirstCommandTarget (this);
  
  //deviceManager->addChangeListener (graphEditor);
}

MainAppWindow::~MainAppWindow()
{
#if JUCE_MAC
  setMacMainMenu (nullptr);
#else
  setMenuBar (nullptr);
#endif

  //deviceManager->removeChangeListener (graphEditor);
  
  audioEngine.getAppProperties().getUserSettings()->setValue ("mainWindowPos", getWindowStateAsString());
  clearContentComponent();
}

void MainAppWindow::closeButtonPressed()
{
  tryToQuitApplication();
}

bool MainAppWindow::tryToQuitApplication()
{
  PluginWindow::closeAllCurrentlyOpenWindows();

  if (getMainComponent() == nullptr
      || getAudioEngine().getDoc().saveIfNeededAndUserAgrees() == FileBasedDocument::savedOk)
  {
    JUCEApplication::quit();
    return true;
  }

  return false;
}

StringArray MainAppWindow::getMenuBarNames()
{
  const char* const names[] = { "File", "Edit", "View", "Options", nullptr };

  return StringArray (names);
}

PopupMenu MainAppWindow::getMenuForIndex (int topLevelMenuIndex, const String& /*menuName*/)
{
  PopupMenu menu;

  if (topLevelMenuIndex == MenuIDs::fileMenu)
  {
    menu.addCommandItem (&getCommandManager(), CommandIDs::newdoc);
    menu.addCommandItem (&getCommandManager(), CommandIDs::open);

    RecentlyOpenedFilesList recentFiles;
    recentFiles.restoreFromString (audioEngine.getAppProperties().getUserSettings()
                                   ->getValue ("recentPMixDocumentFiles"));

    PopupMenu recentFilesMenu;
    recentFiles.createPopupMenuItems (recentFilesMenu, CommandIDs::recentFilesMenu, true, true);
    menu.addSubMenu ("Open recent file", recentFilesMenu);

    menu.addCommandItem (&getCommandManager(), CommandIDs::save);
    menu.addCommandItem (&getCommandManager(), CommandIDs::saveAs);
    menu.addSeparator();
    
#ifndef JUCE_MAC
    menu.addCommandItem (&getCommandManager(), CommandIDs::quit);
#endif
  }
  else if (topLevelMenuIndex == MenuIDs::editMenu)
  {
    menu.addCommandItem (&getCommandManager(), CommandIDs::copy);
    menu.addCommandItem (&getCommandManager(), CommandIDs::paste);
    menu.addCommandItem (&getCommandManager(), CommandIDs::del);
    menu.addSeparator();
    menu.addCommandItem (&getCommandManager(), CommandIDs::undo);
    menu.addCommandItem (&getCommandManager(), CommandIDs::redo);
  }
  else if (topLevelMenuIndex == MenuIDs::viewMenu)
  {
    // "View" menu
    PopupMenu showMenu;
    showMenu.addCommandItem (&getCommandManager(), CommandIDs::showGraphEditor);
    showMenu.addCommandItem (&getCommandManager(), CommandIDs::showISpace);
    showMenu.addCommandItem (&getCommandManager(), CommandIDs::showCodeEditor);
    showMenu.addCommandItem (&getCommandManager(), CommandIDs::showParameters);
    menu.addSubMenu("Show", showMenu);

    PopupMenu floatMenu;
    floatMenu.addItem (CommandIDs::floatGraphEditor, "Graph Editor", true, true);
    floatMenu.addItem (CommandIDs::floatISpace, "Interpolation Space", true, false);
    floatMenu.addItem (CommandIDs::floatCodeEditor, "Code Editor", true, false);
    floatMenu.addItem (CommandIDs::floatParameters, "Parameters", true, false);
    menu.addSubMenu("Float", floatMenu);
    
    menu.addSeparator();
    menu.addCommandItem (&getCommandManager(), CommandIDs::zoomIn);
    menu.addCommandItem (&getCommandManager(), CommandIDs::zoomOut);
    menu.addCommandItem (&getCommandManager(), CommandIDs::zoomNormal);
  }
  else if (topLevelMenuIndex == MenuIDs::optionsMenu)
  {
#ifndef JUCE_MAC
    menu.addCommandItem (&getCommandManager(), CommandIDs::showPrefs);
    menu.addCommandItem (&getCommandManager(), CommandIDs::aboutBox);
#endif
  }

  return menu;
}

void MainAppWindow::menuItemSelected (int menuItemID, int topLevelMenuIndex)
{
  MainComponent* const mainComponent = getMainComponent();

  if (menuItemID >= CommandIDs::recentFilesMenu && menuItemID < (CommandIDs::recentFilesMenu + 100))
  {
    RecentlyOpenedFilesList recentFiles;
    recentFiles.restoreFromString (audioEngine.getAppProperties().getUserSettings()->getValue ("recentPMixDocumentFiles"));

    if (mainComponent != nullptr && audioEngine.getDoc().saveIfNeededAndUserAgrees() == FileBasedDocument::savedOk)
      audioEngine.getDoc().loadFrom (recentFiles.getFile (menuItemID - CommandIDs::recentFilesMenu), true);
  }
}

ApplicationCommandTarget* MainAppWindow::getNextCommandTarget()
{
  return findFirstTargetParentComponent();
}

void MainAppWindow::getAllCommands (Array <CommandID>& commands)
{
  // this returns the set of all commands that this target can perform..
  const CommandID ids[] = {
#ifndef JUCE_MAC
    CommandIDs::quit,
#endif
    CommandIDs::newdoc,
    CommandIDs::open,
    CommandIDs::save,
    CommandIDs::saveAs,
    
    CommandIDs::showPrefs,
    CommandIDs::aboutBox,

    CommandIDs::undo,
    CommandIDs::redo,
    
//    CommandIDs::newAudioInput          ,
//    CommandIDs::newAudioOutput         ,
//    CommandIDs::newMIDIInput           ,
//    CommandIDs::newMIDIOutput          ,
//    CommandIDs::newFaustEffect         ,
    
    CommandIDs::showISpace,
    CommandIDs::showGraphEditor,
    CommandIDs::showCodeEditor,
    CommandIDs::showParameters,
    
    CommandIDs::floatGraphEditor,
    CommandIDs::floatISpace,
    CommandIDs::floatCodeEditor,
    CommandIDs::floatParameters,
  };

  commands.addArray (ids, numElementsInArray (ids));
}

void MainAppWindow::getCommandInfo (const CommandID commandID, ApplicationCommandInfo& result)
{
  const String category ("General");

  switch (commandID)
  {
#ifndef JUCE_MAC
    case CommandIDs::quit:
      result.setInfo ("Quit", "Quit pMix", category, 0);
      result.defaultKeypresses.add (KeyPress ('q', ModifierKeys::commandModifier, 0));
      break;
#endif
    case CommandIDs::newdoc:
      result.setInfo ("New", "Creat a new pMix patch", category, 0);
      result.defaultKeypresses.add (KeyPress ('n', ModifierKeys::commandModifier, 0));
      break;
      
    case CommandIDs::open:
      result.setInfo ("Open...", "Opens a pMix patch", category, 0);
      result.defaultKeypresses.add (KeyPress ('o', ModifierKeys::commandModifier, 0));
      break;

    case CommandIDs::save:
      result.setInfo ("Save", "Saves the current patch", category, 0);
      result.defaultKeypresses.add (KeyPress ('s', ModifierKeys::commandModifier, 0));
      break;

    case CommandIDs::saveAs:
      result.setInfo ("Save As...", "Saves a copy of the patch", category, 0);
      result.defaultKeypresses.add (KeyPress ('s', ModifierKeys::shiftModifier | ModifierKeys::commandModifier, 0));
      break;

    case CommandIDs::aboutBox:
      result.setInfo ("About pMix", String::empty, category, 0);
      break;
      
    case CommandIDs::showPrefs:
      result.setInfo ("Preferences...", String::empty, category, 0);
      result.defaultKeypresses.add (KeyPress (',', ModifierKeys::commandModifier, 0));

      break;
      
    case CommandIDs::undo:
      result.setInfo ("Undo", "Undo the last action", category, 0);
      result.defaultKeypresses.add (KeyPress ('z', ModifierKeys::commandModifier, 0));
      
      break;
    case CommandIDs::redo:
      result.setInfo ("Redo", "Redo the last action", category, 0);
      result.defaultKeypresses.add (KeyPress ('b', ModifierKeys::commandModifier, 0));
      break;
      
    case CommandIDs::showGraphEditor:
      result.setInfo (TRANS("Graph Editor"), TRANS("Shows the Graph Editor"), category, 0);
      //      result.setActive (currentPaintRoutine != nullptr || currentLayout != nullptr);
      //result.defaultKeypresses.add (KeyPress ('1', ModifierKeys::commandModifier, 0));
      break;
    case CommandIDs::showISpace:
      result.setInfo (TRANS("Interpolation Space"), TRANS("Shows the Interpolation Space"), category, 0);
      //      result.setActive (currentPaintRoutine != nullptr || currentLayout != nullptr);
      //result.defaultKeypresses.add (KeyPress ('1', ModifierKeys::commandModifier, 0));
      break;
    case CommandIDs::showCodeEditor:
      result.setInfo (TRANS("Code Editor"), TRANS("Shows the Code Editor"), category, 0);
      //      result.setActive (currentPaintRoutine != nullptr || currentLayout != nullptr);
      //result.defaultKeypresses.add (KeyPress ('1', ModifierKeys::commandModifier, 0));
      break;
    case CommandIDs::showParameters:
      result.setInfo (TRANS("Parameters"), TRANS("Shows the Parameters"), category, 0);
      //      result.setActive (currentPaintRoutine != nullptr || currentLayout != nullptr);
      //result.defaultKeypresses.add (KeyPress ('1', ModifierKeys::commandModifier, 0));
      break;
      
    case CommandIDs::floatGraphEditor:
      result.setInfo (TRANS("Graph Editor"), TRANS("Floats the Graph Editor"), category, 0);
      //      result.setActive (currentPaintRoutine != nullptr || currentLayout != nullptr);
      //result.defaultKeypresses.add (KeyPress ('1', ModifierKeys::commandModifier, 0));
      break;
    case CommandIDs::floatISpace:
      result.setInfo (TRANS("Interpolation Space"), TRANS("Floats the Interpolation Space"), category, 0);
      //      result.setActive (currentPaintRoutine != nullptr || currentLayout != nullptr);
      //result.defaultKeypresses.add (KeyPress ('1', ModifierKeys::commandModifier, 0));
      break;
    case CommandIDs::floatCodeEditor:
      result.setInfo (TRANS("Code Editor"), TRANS("Floats the Code Editor"), category, 0);
      //      result.setActive (currentPaintRoutine != nullptr || currentLayout != nullptr);
      //result.defaultKeypresses.add (KeyPress ('1', ModifierKeys::commandModifier, 0));
      break;
    case CommandIDs::floatParameters:
      result.setInfo (TRANS("Parameters"), TRANS("Floats the Parameters"), category, 0);
      //      result.setActive (currentPaintRoutine != nullptr || currentLayout != nullptr);
      //result.defaultKeypresses.add (KeyPress ('1', ModifierKeys::commandModifier, 0));
      break;
    default:
      break;
  }
}

bool MainAppWindow::perform (const InvocationInfo& info)
{
  MainComponent* const mainComponent = getMainComponent();

  switch (info.commandID)
  {
    case CommandIDs::newdoc:
      if (mainComponent != nullptr && audioEngine.getDoc().saveIfNeededAndUserAgrees() == FileBasedDocument::savedOk)
      {
        audioEngine.getDoc().clear();
        audioEngine.getDoc().initialize();
      }
      break;
      
    case CommandIDs::open:
      if (mainComponent != nullptr && audioEngine.getDoc().saveIfNeededAndUserAgrees() == FileBasedDocument::savedOk)
        audioEngine.getDoc().loadFromUserSpecifiedFile (true);
      break;

    case CommandIDs::save:
      if (mainComponent != nullptr)
        audioEngine.getDoc().save (true, true);
      break;

    case CommandIDs::saveAs:
      if (mainComponent != nullptr)
        audioEngine.getDoc().saveAs (File::nonexistent, true, true, true);
      break;

    case CommandIDs::showPrefs:
      showPreferences();
      break;
    case CommandIDs::aboutBox:
      // TODO
      break;

    case CommandIDs::undo:
      // TODO
      audioEngine.getDoc().getUndoManager().undo();
      break;
      
    case CommandIDs::redo:
      // TODO
      audioEngine.getDoc().getUndoManager().redo();
      break;
      
    default:
      return false;
  }

  return true;
}

bool MainAppWindow::isInterestedInFileDrag (const StringArray&)
{
  return true;
}

void MainAppWindow::fileDragEnter (const StringArray&, int, int)
{
}

void MainAppWindow::fileDragMove (const StringArray&, int, int)
{
}

void MainAppWindow::fileDragExit (const StringArray&)
{
}

void MainAppWindow::filesDropped (const StringArray& files, int x, int y)
{
}

void MainAppWindow::showPreferences()
{
  PMixPrefsComponent prefsComponent(audioEngine);
  
  DialogWindow::LaunchOptions o;
  o.content.setNonOwned (&prefsComponent);
  o.dialogTitle                   = "Preferences";
  o.componentToCentreAround       = getMainComponent();
  o.dialogBackgroundColour        = Colours::grey;
  o.escapeKeyTriggersCloseButton  = true;
  o.useNativeTitleBar             = true;
  o.resizable                     = true;

  o.runModal();

//  ScopedPointer<XmlElement> audioState (getDeviceManager().createStateXml());
//
//  audioEngine.getAppProperties().getUserSettings()->setValue ("audioDeviceState", audioState);
//  audioEngine.getAppProperties().getUserSettings()->saveIfNeeded();
//
//  audioEngine.getDoc().removeIllegalConnections();
}

MainComponent* MainAppWindow::getMainComponent() const
{
  return dynamic_cast <MainComponent*> (getContentComponent());
}
