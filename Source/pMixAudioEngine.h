/*
  ==============================================================================

    pMixAudioEngine.h
    Author:  Oliver Larkin
 
  This class contains the main audio side of pMix and owns the document. It exists before the GUI and should be capable of playing a graph without GUI.

  ==============================================================================
*/

#ifndef PMIXAUDIO_H_INCLUDED
#define PMIXAUDIO_H_INCLUDED

#include "pMixDocument.h"
#include "pMixInternalFilters.h"

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
  PMixDocument &getDoc() noexcept { return doc; }
  ApplicationProperties& getAppProperties() { return *appProperties; }

  void setPluginSortMethod(KnownPluginList::SortMethod sortMethod);
  
  // called from GUI classes to populate menus
  void createDeviceMenu (PopupMenu& m) const;
  const PluginDescription* getChosenType (const int menuID) const;

  //ChangeListener
  void changeListenerCallback (ChangeBroadcaster* broadcaster);

private:
  OwnedArray <PluginDescription> internalTypes;
  KnownPluginList knownPluginList;
  KnownPluginList::SortMethod pluginSortMethod;
  
  AudioProcessorGraph graph;
  AudioPluginFormatManager formatManager;
  
  ScopedPointer<ApplicationProperties> appProperties;

  PMixDocument doc;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PMixAudioEngine)
};




#endif  // PMIXAUDIO_H_INCLUDED
