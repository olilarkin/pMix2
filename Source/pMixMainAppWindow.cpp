#include "../JuceLibraryCode/JuceHeader.h"
#include "pMixMainAppWindow.h"
#include "pMixInternalFilters.h"

static double snapToIntegerZoom (double zoom)
{
  if (zoom >= 1.0)
    return (double) (int) (zoom + 0.5);
  
  return 1.0 / (int) (1.0 / zoom + 0.5);
}

//class MainAppWindow::PluginListWindow  : public DocumentWindow
//{
//public:
//  PluginListWindow (MainAppWindow& owner_, AudioPluginFormatManager& formatManager)
//    : DocumentWindow ("Available Plugins", Colours::white,
//                      DocumentWindow::minimiseButton | DocumentWindow::closeButton),
//    owner (owner_)
//  {
//    const File deadMansPedalFile (getAppProperties().getUserSettings()
//                                  ->getFile().getSiblingFile ("RecentlyCrashedPluginsList"));
//
////    setContentOwned (new PluginListComponent (formatManager,
////                     owner.knownPluginList,
////                     deadMansPedalFile,
////                     getAppProperties().getUserSettings()), true);
//
//    setResizable (true, false);
//    setResizeLimits (300, 400, 800, 1500);
//    setTopLeftPosition (60, 60);
//
//    restoreWindowStateFromString (getAppProperties().getUserSettings()->getValue ("listWindowPos"));
//    setVisible (true);
//  }
//
//  ~PluginListWindow()
//  {
//    getAppProperties().getUserSettings()->setValue ("listWindowPos", getWindowStateAsString());
//
//    clearContentComponent();
//  }
//
//  void closeButtonPressed()
//  {
//    owner.pluginListWindow = nullptr;
//  }
//
//private:
//  MainAppWindow& owner;
//
//  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginListWindow)
//};

MainAppWindow::MainAppWindow(AudioDeviceManager* deviceManager)
  : DocumentWindow (JUCEApplication::getInstance()->getApplicationName(), Colours::lightgrey, DocumentWindow::allButtons)
  , deviceManager(deviceManager)
{

  setUsingNativeTitleBar (true);
  setResizable (true, false);
  setResizeLimits (500, 400, 10000, 10000);
  centreWithSize (800, 600);

  setContentOwned (new MainComponent (audio), false);

//  restoreWindowStateFromString (getAppProperties().getUserSettings()->getValue ("mainWindowPos"));

  setVisible (true);

  addKeyListener (getCommandManager().getKeyMappings());

  Process::setPriority (Process::HighPriority);
  
  getCommandManager().registerAllCommandsForTarget (this);

  PopupMenu pop;
  pop.addCommandItem (&getCommandManager(), CommandIDs::aboutBox);
  pop.addSeparator();
  pop.addCommandItem (&getCommandManager(), CommandIDs::showAudioSettings);
  pop.addSeparator();
  
#if JUCE_MAC
  setMacMainMenu (this, &pop);
#else
  setMenuBar (this);
#endif
  
  getCommandManager().setFirstCommandTarget (this);
  
  deviceManager->addAudioCallback (&audio.getGraphPlayer());
  deviceManager->addMidiInputCallback (String::empty, &audio.getGraphPlayer().getMidiMessageCollector());
  //deviceManager->addChangeListener (graphEditor);
}

MainAppWindow::~MainAppWindow()
{
//  pluginListWindow = nullptr;

#if JUCE_MAC
  setMacMainMenu (nullptr);
#else
  setMenuBar (nullptr);
#endif

  deviceManager->removeAudioCallback (&audio.getGraphPlayer());
  deviceManager->removeMidiInputCallback (String::empty, &audio.getGraphPlayer().getMidiMessageCollector());
  //deviceManager->removeChangeListener (graphEditor);
  
  getAppProperties().getUserSettings()->setValue ("mainWindowPos", getWindowStateAsString());
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
      || getAudio().getDoc().saveIfNeededAndUserAgrees() == FileBasedDocument::savedOk)
  {
    JUCEApplication::quit();
    return true;
  }

  return false;
}

StringArray MainAppWindow::getMenuBarNames()
{
  const char* const names[] = { "File", "Edit", "View", nullptr };

  return StringArray (names);
}

PopupMenu MainAppWindow::getMenuForIndex (int topLevelMenuIndex, const String& /*menuName*/)
{
  PopupMenu menu;

  if (topLevelMenuIndex == 0)
  {
    // "File" menu
    menu.addCommandItem (&getCommandManager(), CommandIDs::open);

    RecentlyOpenedFilesList recentFiles;
    recentFiles.restoreFromString (getAppProperties().getUserSettings()
                                   ->getValue ("recentPMixDocumentFiles"));

    PopupMenu recentFilesMenu;
    recentFiles.createPopupMenuItems (recentFilesMenu, 100, true, true);
    menu.addSubMenu ("Open recent file", recentFilesMenu);

    menu.addCommandItem (&getCommandManager(), CommandIDs::save);
    menu.addCommandItem (&getCommandManager(), CommandIDs::saveAs);
    //menu.addSeparator();
    //menu.addCommandItem (&getCommandManager(), StandardApplicationCommandIDs::quit);
  }
  else if (topLevelMenuIndex == 1)
  {
    menu.addCommandItem (&getCommandManager(), CommandIDs::copy);
    menu.addCommandItem (&getCommandManager(), CommandIDs::paste);
    menu.addSeparator();
    menu.addCommandItem (&getCommandManager(), CommandIDs::undo);
    menu.addCommandItem (&getCommandManager(), CommandIDs::redo);
  }
  else if (topLevelMenuIndex == 2)
  {
    // "View" menu
    PopupMenu showMenu;
    showMenu.addCommandItem (&getCommandManager(), CommandIDs::showGraphEditor);
    showMenu.addCommandItem (&getCommandManager(), CommandIDs::showISpace);
    showMenu.addCommandItem (&getCommandManager(), CommandIDs::showCodeEditor);
    showMenu.addCommandItem (&getCommandManager(), CommandIDs::showParameters);
    menu.addSubMenu("Show", showMenu);

    PopupMenu floatMenu;
    floatMenu.addItem (263, "Graph Editor", true, true);
    floatMenu.addItem (264, "Interpolation Space", true, false);
    floatMenu.addItem (265, "Code Editor", true, false);
    floatMenu.addItem (266, "Parameters", true, false);
    menu.addSubMenu("Float", floatMenu);
    
    menu.addSeparator();
    menu.addCommandItem (&getCommandManager(), CommandIDs::zoomIn);
    menu.addCommandItem (&getCommandManager(), CommandIDs::zoomOut);
    menu.addCommandItem (&getCommandManager(), CommandIDs::zoomNormal);
  }
//  else if (topLevelMenuIndex == 3)
//  {
//    // "Plugins" menu
//    PopupMenu pluginsMenu;
//    addPluginsToMenu (pluginsMenu);
//    menu.addSubMenu ("Create plugin", pluginsMenu);
//    menu.addSeparator();
//    menu.addItem (250, "Delete all plugins");
//  }
//  else if (topLevelMenuIndex == 4)
//  {
//    // "Options" menu
//
//    menu.addCommandItem (&getCommandManager(), CommandIDs::showPluginListEditor);
//
//    KnownPluginList::SortMethod pluginSortMethod = audio.getSortMethod();
//    
//    PopupMenu sortTypeMenu;
//    sortTypeMenu.addItem (200, "List plugins in default order",      true, pluginSortMethod == KnownPluginList::defaultOrder);
//    sortTypeMenu.addItem (201, "List plugins in alphabetical order", true, pluginSortMethod == KnownPluginList::sortAlphabetically);
//    sortTypeMenu.addItem (202, "List plugins by category",           true, pluginSortMethod == KnownPluginList::sortByCategory);
//    sortTypeMenu.addItem (203, "List plugins by manufacturer",       true, pluginSortMethod == KnownPluginList::sortByManufacturer);
//    sortTypeMenu.addItem (204, "List plugins based on the directory structure", true, pluginSortMethod == KnownPluginList::sortByFileSystemLocation);
//    menu.addSubMenu ("Plugin menu type", sortTypeMenu);
//  }

  return menu;
}

void MainAppWindow::menuItemSelected (int menuItemID, int /*topLevelMenuIndex*/)
{
  MainComponent* const mainComponent = getMainComponent();

  if (menuItemID == 250)
  {
    if (mainComponent != nullptr)
      audio.getDoc().clear();
  }
  else if (menuItemID >= 100 && menuItemID < 200)
  {
    RecentlyOpenedFilesList recentFiles;
    recentFiles.restoreFromString (getAppProperties().getUserSettings()
                                   ->getValue ("recentPMixDocumentFiles"));

    if (mainComponent != nullptr && audio.getDoc().saveIfNeededAndUserAgrees() == FileBasedDocument::savedOk)
      audio.getDoc().loadFrom (recentFiles.getFile (menuItemID - 100), true);
  }
//  else if (menuItemID >= 200 && menuItemID < 210)
//  {
//    if (menuItemID == 200)          audio.setPluginSortMethod(KnownPluginList::defaultOrder);
//    else if (menuItemID == 201)     audio.setPluginSortMethod(KnownPluginList::sortAlphabetically);
//    else if (menuItemID == 202)     audio.setPluginSortMethod(KnownPluginList::sortByCategory);
//    else if (menuItemID == 203)     audio.setPluginSortMethod(KnownPluginList::sortByManufacturer);
//    else if (menuItemID == 204)     audio.setPluginSortMethod(KnownPluginList::sortByFileSystemLocation);
//
//    menuItemsChanged();
//  }
//  else
//  {
//    createPlugin (getChosenType (menuItemID),
//                  proportionOfWidth  (0.3f + Random::getSystemRandom().nextFloat() * 0.6f),
//                  proportionOfHeight (0.3f + Random::getSystemRandom().nextFloat() * 0.6f));
//  }
}

ApplicationCommandTarget* MainAppWindow::getNextCommandTarget()
{
  return findFirstTargetParentComponent();
}

void MainAppWindow::getAllCommands (Array <CommandID>& commands)
{
  // this returns the set of all commands that this target can perform..
  const CommandID ids[] = { 
                            CommandIDs::aboutBox,
                            CommandIDs::open,
                            CommandIDs::save,
                            CommandIDs::saveAs,
                            CommandIDs::showPluginListEditor,
                            CommandIDs::showAudioSettings,
                            CommandIDs::copy,
                            CommandIDs::paste,
                            CommandIDs::undo,
                            CommandIDs::redo,
                            CommandIDs::zoomIn,
                            CommandIDs::zoomOut,
                            CommandIDs::zoomNormal,
                            CommandIDs::showGraphEditor,
                            CommandIDs::showISpace,
                            CommandIDs::showCodeEditor,
                            CommandIDs::showParameters
                          };

  commands.addArray (ids, numElementsInArray (ids));
}

void MainAppWindow::getCommandInfo (const CommandID commandID, ApplicationCommandInfo& result)
{
  const String category ("General");

  switch (commandID)
  {
    case CommandIDs::open:
      result.setInfo ("Open...",
                      "Opens a pMix patch",
                      category, 0);
      result.defaultKeypresses.add (KeyPress ('o', ModifierKeys::commandModifier, 0));
      break;

    case CommandIDs::save:
      result.setInfo ("Save",
                      "Saves the current patch",
                      category, 0);
      result.defaultKeypresses.add (KeyPress ('s', ModifierKeys::commandModifier, 0));
      break;

    case CommandIDs::saveAs:
      result.setInfo ("Save As...",
                      "Saves a copy of the patch",
                      category, 0);
      result.defaultKeypresses.add (KeyPress ('s', ModifierKeys::shiftModifier | ModifierKeys::commandModifier, 0));
      break;

    case CommandIDs::showPluginListEditor:
      result.setInfo ("Edit the list of available plug-Ins...", String::empty, category, 0);
      result.addDefaultKeypress ('p', ModifierKeys::commandModifier);
      break;

    case CommandIDs::showAudioSettings:
      result.setInfo ("Preferences...", String::empty, category, 0);
      result.addDefaultKeypress (',', ModifierKeys::commandModifier);
      break;

    case CommandIDs::aboutBox:
      result.setInfo ("About pMix", String::empty, category, 0);
      break;
      
    case CommandIDs::copy:
      result.setInfo ("Copy",
                      "Copies the currently selected filter to the clipboard",
                      category, 0);
      result.defaultKeypresses.add (KeyPress ('c', ModifierKeys::commandModifier, 0));
      
      break;
    case CommandIDs::paste:
      result.setInfo ("Paste",
                      "Pastes from the clipboard",
                      category, 0);
      result.defaultKeypresses.add (KeyPress ('p', ModifierKeys::commandModifier, 0));
      
      break;
    case CommandIDs::undo:
      result.setInfo ("Undo",
                      "Undo the last action",
                      category, 0);
      result.defaultKeypresses.add (KeyPress ('z', ModifierKeys::commandModifier, 0));
      
      break;
    case CommandIDs::redo:
      result.setInfo ("Redo",
                      "Redo the last action",
                      category, 0);
      result.defaultKeypresses.add (KeyPress ('b', ModifierKeys::commandModifier, 0));
      break;
    case CommandIDs::zoomIn:
      result.setInfo (TRANS("Zoom in"), TRANS("Zooms in on the current component."), category, 0);
//      result.setActive (currentPaintRoutine != nullptr || currentLayout != nullptr);
      result.defaultKeypresses.add (KeyPress (']', ModifierKeys::commandModifier, 0));
      break;
      
    case CommandIDs::zoomOut:
      result.setInfo (TRANS("Zoom out"), TRANS("Zooms out on the current component."), category, 0);
//      result.setActive (currentPaintRoutine != nullptr || currentLayout != nullptr);
      result.defaultKeypresses.add (KeyPress ('[', ModifierKeys::commandModifier, 0));
      break;
      
    case CommandIDs::zoomNormal:
      result.setInfo (TRANS("Zoom to 100%"), TRANS("Restores the zoom level to normal."), category, 0);
//      result.setActive (currentPaintRoutine != nullptr || currentLayout != nullptr);
      result.defaultKeypresses.add (KeyPress ('1', ModifierKeys::commandModifier, 0));
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
      //TODO: use these:
//    case StandardApplicationCommandIDs::undo:
//      break;
//      
//    case StandardApplicationCommandIDs::redo:
//      break;
//      
//    case StandardApplicationCommandIDs::cut:
//      break;
//      
//    case StandardApplicationCommandIDs::copy:
//      break;
//      
//    case StandardApplicationCommandIDs::paste:
//      break;
//      
//    case StandardApplicationCommandIDs::del:
//      break;
//      
//    case StandardApplicationCommandIDs::selectAll:
//      break;
//      
//    case StandardApplicationCommandIDs::deselectAll:
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
    case CommandIDs::open:
      if (mainComponent != nullptr && audio.getDoc().saveIfNeededAndUserAgrees() == FileBasedDocument::savedOk)
        audio.getDoc().loadFromUserSpecifiedFile (true);

      break;

    case CommandIDs::save:
      if (mainComponent != nullptr)
        audio.getDoc().save (true, true);
      break;

    case CommandIDs::saveAs:
      if (mainComponent != nullptr)
        audio.getDoc().saveAs (File::nonexistent, true, true, true);
      break;

    case CommandIDs::showPluginListEditor:
//      if (pluginListWindow == nullptr)
//        pluginListWindow = new PluginListWindow (*this, formatManager);

//      pluginListWindow->toFront (true);
      break;

    case CommandIDs::showAudioSettings:
      //showAudioSettings();
      break;

    case CommandIDs::aboutBox:
      // TODO
      break;
      
    case CommandIDs::copy:
      // TODO
      break;

    case CommandIDs::paste:
      // TODO
      break;
      
    case CommandIDs::undo:
      // TODO
      audio.getDoc().getUndoManager().undo();
      break;
      
    case CommandIDs::redo:
      // TODO
      audio.getDoc().getUndoManager().redo();
      break;
      
    case CommandIDs::zoomIn:      getMainComponent()->setZoom (snapToIntegerZoom (getMainComponent()->getZoom() * 2.0)); break;
    case CommandIDs::zoomOut:     getMainComponent()->setZoom (snapToIntegerZoom (getMainComponent()->getZoom() / 2.0)); break;
    case CommandIDs::zoomNormal:  getMainComponent()->setZoom (1.0); break;
      
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
  MainComponent* const mainComponent = getMainComponent();

  if (mainComponent != nullptr)
  {
//    if (files.size() == 1 && File (files[0]).hasFileExtension (filenameSuffix))
//    {
//      if (audio.getDoc().saveIfNeededAndUserAgrees() == FileBasedDocument::savedOk)
//        audio.getDoc().loadFrom (File (files[0]), true);
//    }
//    else
//    {
//      OwnedArray <PluginDescription> typesFound;
//      audio.getKnownPluginList().scanAndAddDragAndDroppedFiles (formatManager, files, typesFound);
//
//      Point<int> pos (mainComponent->getLocalPoint (this, Point<int> (x, y)));
//
//      for (int i = 0; i < jmin (5, typesFound.size()); ++i)
//        createPlugin (typesFound.getUnchecked(i), pos.getX(), pos.getY());
//    }
  }
}

MainComponent* MainAppWindow::getMainComponent() const
{
  return dynamic_cast <MainComponent*> (getContentComponent());
}
