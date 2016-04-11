/*
  ==============================================================================

    pMixAudioEngine.h
    Author:  Oliver Larkin
 
    This class contains the main audio side of pMix and owns the document. It exists before the GUI and should be capable of playing a graph without GUI.

  ==============================================================================
*/

#ifndef PMIXAUDIOENGINE_H_INCLUDED
#define PMIXAUDIOENGINE_H_INCLUDED

#include "pMixDocument.h"
#include "pMixInternalPluginFormat.h"
#include "pMixLogger.h"

ApplicationCommandManager& getCommandManager();

class PMixAudioEngine : public ChangeListener
{
public:
  PMixAudioEngine();
  ~PMixAudioEngine();
  
  AudioProcessorGraph& getGraph() noexcept { return graph; }
  //AudioProcessorPlayer& getGraphPlayer() noexcept { return graphPlayer; }
  AudioPluginFormatManager& getFormatManager() noexcept { return formatManager; }

  KnownPluginList& getKnownPluginList() { return knownPluginList; }
  KnownPluginList::SortMethod getSortMethod() noexcept { return pluginSortMethod; }
  const File getDMPFile() { return getAppProperties().getUserSettings()->getFile().getSiblingFile ("RecentlyCrashedPluginsList"); }
  
  PMixDocument &getDoc() noexcept { return doc; }
  PMixLogger &getLogger() noexcept { return logger; }

  ApplicationProperties& getAppProperties() { return *appProperties; }

  void setPluginSortMethod(KnownPluginList::SortMethod sortMethod);
  
  // called from GUI classes to populate menus
  void createNodeMenu (PopupMenu& m) const;
  const PluginDescription* getChosenType (const int menuID) const;

  //ChangeListener
  void changeListenerCallback (ChangeBroadcaster* broadcaster);
  
  AudioPluginInstance* createPluginInstance(const PluginDescription& desc, String& errorMessage);
  
private:
  PMixDocument doc;

  AudioProcessorGraph graph;
  AudioPluginFormatManager formatManager; // for internal I/O and VST plugins etc
  AudioPluginFormatManager JITformatManager; // for FAUST formats ...
  
  OwnedArray <PluginDescription> internalTypes; // includes FAUST plugin type

  KnownPluginList knownPluginList;
  KnownPluginList::SortMethod pluginSortMethod;
  
  KnownPluginList knownFaustDSPList;
  
  PMixLogger logger;

  ScopedPointer<ApplicationProperties> appProperties;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PMixAudioEngine)
};




#endif  // PMIXAUDIOENGINE_H_INCLUDED
