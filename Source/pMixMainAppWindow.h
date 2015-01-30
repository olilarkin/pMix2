/*
 ==============================================================================
 
 pMixMainAppWindow.h
 Author:  Oliver Larkin
 
 The top level window for the app.
 
 ==============================================================================
 */

#ifndef PMIXMAINAPPWINDOW_H_INCLUDED
#define PMIXMAINAPPWINDOW_H_INCLUDED

#include "pMixAudio.h"
#include "pMixMainComponent.h"
#include "pMixCommandIDs.h"

ApplicationCommandManager& getCommandManager();
ApplicationProperties& getAppProperties();

class MainAppWindow : public DocumentWindow,
                      public MenuBarModel,
                      public ApplicationCommandTarget,
                      public FileDragAndDropTarget
{
public:
  MainAppWindow(PMixAudio& audio);
  ~MainAppWindow();

  //DocumentWindow
  void closeButtonPressed();
  
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
  MainComponent* getMainComponent() const;
  PMixAudio& getAudio() { return audio; }
private:
  PMixAudio& audio;
  
  class PluginListWindow;
  ScopedPointer <PluginListWindow> pluginListWindow;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainAppWindow)
};


#endif   // PMIXMAINAPPWINDOW_H_INCLUDED
