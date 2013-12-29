#ifndef __MAINHOSTWINDOW_JUCEHEADER__
#define __MAINHOSTWINDOW_JUCEHEADER__

#include "FilterGraph.h"
#include "GraphEditorPanel.h"


namespace CommandIDs
{
  static const int open                   = 0x30000;
  static const int save                   = 0x30001;
  static const int saveAs                 = 0x30002;
  static const int showPluginListEditor   = 0x30100;
  static const int showAudioSettings      = 0x30200;
  static const int aboutBox               = 0x30300;
  static const int copy                   = 0x30010;
  static const int paste                  = 0x30020;
  static const int undo                   = 0x30030;
  static const int redo                   = 0x30040;
}

ApplicationCommandManager& getCommandManager();
ApplicationProperties& getAppProperties();

class MainHostWindow    : public DocumentWindow,
  public MenuBarModel,
  public ApplicationCommandTarget,
  public ChangeListener,
  public FileDragAndDropTarget
{
public:
  MainHostWindow();
  ~MainHostWindow();

  void closeButtonPressed();
  void changeListenerCallback (ChangeBroadcaster*);

  bool isInterestedInFileDrag (const StringArray& files);
  void fileDragEnter (const StringArray& files, int, int);
  void fileDragMove (const StringArray& files, int, int);
  void fileDragExit (const StringArray& files);
  void filesDropped (const StringArray& files, int, int);

  StringArray getMenuBarNames();
  PopupMenu getMenuForIndex (int topLevelMenuIndex, const String& menuName);
  void menuItemSelected (int menuItemID, int topLevelMenuIndex);
  ApplicationCommandTarget* getNextCommandTarget();
  void getAllCommands (Array <CommandID>& commands);
  void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result);
  bool perform (const InvocationInfo& info);

  bool tryToQuitApplication();

  void createPlugin (const PluginDescription* desc, int x, int y);

  void addPluginsToMenu (PopupMenu& m) const;
  const PluginDescription* getChosenType (const int menuID) const;

  GraphDocumentComponent* getGraphEditor() const;

private:
  AudioDeviceManager deviceManager;
  AudioPluginFormatManager formatManager;

  OwnedArray <PluginDescription> internalTypes;
  KnownPluginList knownPluginList;
  KnownPluginList::SortMethod pluginSortMethod;

  class PluginListWindow;
  ScopedPointer <PluginListWindow> pluginListWindow;

  void showAudioSettings();

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainHostWindow)
};


#endif   // __MAINHOSTWINDOW_JUCEHEADER__
