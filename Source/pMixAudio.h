/*
  ==============================================================================

    pMixAudio.h
    Author:  Oliver Larkin
 
  This class contains the main audio side of pMix and owns the document. It exists before the GUI.

  ==============================================================================
*/

#ifndef PMIXAUDIO_H_INCLUDED
#define PMIXAUDIO_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "pMixDocument.h"
#include "pMixInternalFilters.h"

class PMixAudio : public ChangeListener
{
public:
  PMixAudio();
  ~PMixAudio();
  
  AudioProcessorGraph& getGraph() noexcept { return graph; }
  AudioProcessorPlayer& getGraphPlayer() noexcept { return graphPlayer; }
  AudioPluginFormatManager& getFormatManager() noexcept { return formatManager; }
  PMixDocument &getDocument() noexcept { return doc; }

  //ChangeListener
  void changeListenerCallback (ChangeBroadcaster* broadcaster);

private:
  OwnedArray <PluginDescription> internalTypes;
  KnownPluginList knownPluginList;
  KnownPluginList::SortMethod pluginSortMethod;
  
  AudioProcessorGraph graph;
  AudioProcessorPlayer graphPlayer;
  AudioPluginFormatManager formatManager;
  
  PMixDocument doc;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PMixAudio)
};




#endif  // PMIXAUDIO_H_INCLUDED
