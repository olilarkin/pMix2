#ifndef __MAINAPPWINDOW_JUCEHEADER__
#define __MAINAPPWINDOW_JUCEHEADER__

#include "PMixDocument.h"
#include "MainComponent.h"
#include "CommandIDs.h"

ApplicationCommandManager& getCommandManager();
ApplicationProperties& getAppProperties();

class MainAppWindow    : public DocumentWindow,
                          public MenuBarModel,
                          public ApplicationCommandTarget,
                          public ChangeListener,
                          public FileDragAndDropTarget
{
public:
  MainAppWindow(AudioDeviceManager* deviceManager);
  ~MainAppWindow();

  //DocumentWindow
  void closeButtonPressed();
  
  //ChangeListener
  void changeListenerCallback (ChangeBroadcaster*);

  //FileDragAndDropTarget
  bool isInterestedInFileDrag (const StringArray& files);
  void fileDragEnter (const StringArray& files, int, int);
  void fileDragMove (const StringArray& files, int, int);
  void fileDragExit (const StringArray& files);
  void filesDropped (const StringArray& files, int, int);

  //MenuBarModel
  StringArray getMenuBarNames();
  PopupMenu getMenuForIndex (int topLevelMenuIndex, const String& menuName);
  void menuItemSelected (int menuItemID, int topLevelMenuIndex);
  
  //ApplicationCommandTarget
  ApplicationCommandTarget* getNextCommandTarget();
  void getAllCommands (Array <CommandID>& commands);
  void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result);
  bool perform (const InvocationInfo& info);

  //Unique
  bool tryToQuitApplication();
  void createPlugin (const PluginDescription* desc, int x, int y);
  void addPluginsToMenu (PopupMenu& m) const;
  const PluginDescription* getChosenType (const int menuID) const;
  MainComponent* getMainComponent() const;

private:
  OwnedArray <PluginDescription> internalTypes;
  KnownPluginList knownPluginList;
  KnownPluginList::SortMethod pluginSortMethod;
  AudioPluginFormatManager formatManager;
  AudioDeviceManager* deviceManager;
  
  class PluginListWindow;
  ScopedPointer <PluginListWindow> pluginListWindow;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainAppWindow)
};


#endif   // __MAINAPPWINDOW_JUCEHEADER__
