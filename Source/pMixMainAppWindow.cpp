/*
  ==============================================================================
 
    pMixMainAppWindow.cpp
    Author:  Oliver Larkin
  
  ==============================================================================
 */
 
#include "pMixMainAppWindow.h"
#include "pMixPrefsComponent.h"
#include "pMixAboutBox.h"

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
  
  setApplicationCommandManagerToWatch(&getCommandManager());
}

MainAppWindow::~MainAppWindow()
{
#if JUCE_MAC
  setMacMainMenu (nullptr);
#else
  setMenuBar (nullptr);
#endif
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
#if JUCE_LINUX || JUCE_WIN
  const char* const names[] = { "File", "Edit", "View", "Options", "Code", nullptr };
#else
  const char* const names[] = { "File", "Edit", "View", "Code", nullptr };
#endif
  return StringArray (names);
}

PopupMenu MainAppWindow::getMenuForIndex (int topLevelMenuIndex, const String& menuName)
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
    menu.addSubMenu ("Open Recent", recentFilesMenu);
    menu.addSeparator();
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
    menu.addCommandItem (&getCommandManager(), CommandIDs::selectAll);
    menu.addSeparator();
    menu.addCommandItem (&getCommandManager(), CommandIDs::undo);
    menu.addCommandItem (&getCommandManager(), CommandIDs::redo);
  }
  else if (topLevelMenuIndex == MenuIDs::viewMenu)
  {
    // "View" menu
    PopupMenu showMenu;
    showMenu.addCommandItem (&getCommandManager(), CommandIDs::showConsole);
    showMenu.addCommandItem (&getCommandManager(), CommandIDs::showISpace);
    showMenu.addCommandItem (&getCommandManager(), CommandIDs::showCodeEditor);
//    showMenu.addCommandItem (&getCommandManager(), CommandIDs::showParameters);
    menu.addSubMenu("Show", showMenu);

    PopupMenu floatMenu;
    floatMenu.addCommandItem (&getCommandManager(), CommandIDs::floatConsole);
    floatMenu.addCommandItem (&getCommandManager(), CommandIDs::floatISpace);
    floatMenu.addCommandItem (&getCommandManager(), CommandIDs::floatCodeEditor);
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
  else if (topLevelMenuIndex == MenuIDs::codeMenu)
  {
    menu.addCommandItem (&getCommandManager(), CommandIDs::compile);
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
    {
      getMainComponent()->clear();
      audioEngine.getDoc().loadFrom (recentFiles.getFile (menuItemID - CommandIDs::recentFilesMenu), true);
    }
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

//    CommandIDs::copy,
//    CommandIDs::paste,
//    CommandIDs::del,
//    CommandIDs::selectAll,

    CommandIDs::undo,
    CommandIDs::redo,
    
//    CommandIDs::newAudioInput          ,
//    CommandIDs::newAudioOutput         ,
//    CommandIDs::newMIDIInput           ,
//    CommandIDs::newMIDIOutput          ,
//    CommandIDs::newFaustEffect         ,
    
    CommandIDs::showISpace,
    CommandIDs::showConsole,
    CommandIDs::showCodeEditor,
//    CommandIDs::showParameters,
    
    CommandIDs::floatConsole,
    CommandIDs::floatISpace,
    CommandIDs::floatCodeEditor,
//    CommandIDs::floatParameters,
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
      result.setInfo ("About pMix", String(), category, 0);
      break;
      
    case CommandIDs::showPrefs:
      result.setInfo ("Preferences...", String(), category, 0);
      result.defaultKeypresses.add (KeyPress (',', ModifierKeys::commandModifier, 0));
      break;
      
//    case CommandIDs::copy:
//      result.setInfo ("Copy", "Copy to clipboard", category, 0);
//      result.defaultKeypresses.add (KeyPress ('c', ModifierKeys::commandModifier, 0));
//      break;
//    case CommandIDs::paste:
//      result.setInfo ("Paste", "Paste from clipboard", category, 0);
//      result.defaultKeypresses.add (KeyPress ('v', ModifierKeys::commandModifier, 0));
//      break;
//      
//    case CommandIDs::del:
//      result.setInfo ("Delete", "Delete selected", category, 0);
//      result.defaultKeypresses.add (KeyPress (KeyPress::backspaceKey, 0, 0));
//      break;
//      
//    case CommandIDs::selectAll:
//      result.setInfo ("Select All", "Select All", category, 0);
//      result.defaultKeypresses.add (KeyPress ('a', ModifierKeys::commandModifier, 0));
//      break;
      
    case CommandIDs::undo:
      result.setInfo ("Undo", "Undo the last action", category, 0);
      result.defaultKeypresses.add (KeyPress ('z', ModifierKeys::commandModifier, 0));
      
      break;
    case CommandIDs::redo:
      result.setInfo ("Redo", "Redo the last action", category, 0);
      result.defaultKeypresses.add (KeyPress ('b', ModifierKeys::commandModifier, 0));
      break;
      
    case CommandIDs::showConsole:
      result.setInfo (TRANS("Console"), TRANS("Shows the Console"), category, 0);
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
//    case CommandIDs::showParameters:
//      result.setInfo (TRANS("Parameters"), TRANS("Shows the Parameters"), category, 0);
      //      result.setActive (currentPaintRoutine != nullptr || currentLayout != nullptr);
      //result.defaultKeypresses.add (KeyPress ('1', ModifierKeys::commandModifier, 0));
//      break;
      
    case CommandIDs::floatConsole:
      result.setInfo (TRANS("Console"), TRANS("Floats the Console"), category, 0);
      result.setTicked(audioEngine.getAppProperties().getUserSettings()->getBoolValue("floatConsole"));
      //      result.setActive (currentPaintRoutine != nullptr || currentLayout != nullptr);
      //result.defaultKeypresses.add (KeyPress ('1', ModifierKeys::commandModifier, 0));
      break;
    case CommandIDs::floatISpace:
      result.setInfo (TRANS("Interpolation Space"), TRANS("Floats the Interpolation Space"), category, 0);
      result.setTicked(audioEngine.getAppProperties().getUserSettings()->getBoolValue("floatInterpolationSpace"));
      //      result.setActive (currentPaintRoutine != nullptr || currentLayout != nullptr);
      //result.defaultKeypresses.add (KeyPress ('1', ModifierKeys::commandModifier, 0));
      break;
    case CommandIDs::floatCodeEditor:
      result.setInfo (TRANS("Code Editor"), TRANS("Floats the Code Editor"), category, 0);
      result.setTicked(audioEngine.getAppProperties().getUserSettings()->getBoolValue("floatCodeEditor"));
      //      result.setActive (currentPaintRoutine != nullptr || currentLayout != nullptr);
      //result.defaultKeypresses.add (KeyPress ('1', ModifierKeys::commandModifier, 0));
      break;
//    case CommandIDs::floatParameters:
//      result.setInfo (TRANS("Parameters"), TRANS("Floats the Parameters"), category, 0);
//      //      result.setActive (currentPaintRoutine != nullptr || currentLayout != nullptr);
//      //result.defaultKeypresses.add (KeyPress ('1', ModifierKeys::commandModifier, 0));
//      break;
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
        getMainComponent()->clear();
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
        audioEngine.getDoc().saveAs ({}, true, true, true);
      break;

    case CommandIDs::showPrefs:
      showPreferences();
      break;
    case CommandIDs::aboutBox:
      showAbout();
      break;

//    case CommandIDs::copy:
//      // TODO
//      break;
//    case CommandIDs::paste:
//      // TODO
//      break;
//      
//    case CommandIDs::del:
//      // TODO
//      break;
//    case CommandIDs::selectAll:
//      // TODO
//      break;
      
    case CommandIDs::undo:
      // TODO
      audioEngine.getDoc().getUndoManager().undo();
      break;
      
    case CommandIDs::redo:
      // TODO
      audioEngine.getDoc().getUndoManager().redo();
      break;
      
    case CommandIDs::showISpace:
      break;
      
    case CommandIDs::showConsole:
      break;
      
    case CommandIDs::showCodeEditor:
      break;
      
    case CommandIDs::floatISpace:
      getMainComponent()->floatWindow(MainComponent::kWindowISpace,
                                      !(audioEngine.getAppProperties().getUserSettings()->getBoolValue("floatInterpolationSpace")));
      break;
      
    case CommandIDs::floatConsole:
      break;
      
    case CommandIDs::floatCodeEditor:
      getMainComponent()->floatWindow(MainComponent::kWindowCodeEditor,
                                      !(audioEngine.getAppProperties().getUserSettings()->getBoolValue("floatCodeEditor")));
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

  std::unique_ptr<XmlElement> audioState (getDeviceManager().createStateXml());

//  audioEngine.getAppProperties().getUserSettings()->setValue("audioDeviceState", audioState); //TODO:
  audioEngine.getAppProperties().getUserSettings()->saveIfNeeded();

  audioEngine.getDoc().removeIllegalConnections();
}

void MainAppWindow::showAbout()
{
  pMixAboutBox aboutBoxComponent;
  
  DialogWindow::LaunchOptions o;
  o.content.setNonOwned (&aboutBoxComponent);
  o.dialogTitle                   = "About pMix";
  o.componentToCentreAround       = getMainComponent();
  o.dialogBackgroundColour        = Colours::white;
  o.escapeKeyTriggersCloseButton  = true;
  o.useNativeTitleBar             = true;
  o.resizable                     = false;
  
  o.runModal();
}

MainComponent* MainAppWindow::getMainComponent() const
{
  return dynamic_cast <MainComponent*> (getContentComponent());
}
