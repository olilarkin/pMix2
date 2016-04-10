/*
  ==============================================================================

    pMixInternalPluginFormat.cpp
    Author:  Oliver Larkin
 
  ==============================================================================
*/

#include "pMixInternalPluginFormat.h"

InternalPluginFormat::InternalPluginFormat()
{
  {
    AudioProcessorGraph::AudioGraphIOProcessor p (AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode);
    p.fillInPluginDescription (audioOutDesc);
  }

  {
    AudioProcessorGraph::AudioGraphIOProcessor p (AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode);
    p.fillInPluginDescription (audioInDesc);
  }

  {
    AudioProcessorGraph::AudioGraphIOProcessor p (AudioProcessorGraph::AudioGraphIOProcessor::midiInputNode);
    p.fillInPluginDescription (midiInDesc);
  }
  
  {
    AudioProcessorGraph::AudioGraphIOProcessor p (AudioProcessorGraph::AudioGraphIOProcessor::midiOutputNode);
    p.fillInPluginDescription (midiOutDesc);
  }
}

void InternalPluginFormat::createPluginInstance (const PluginDescription& desc,
                                                 double initialSampleRate,
                                                 int initialBufferSize,
                                                 void* userData,
                                                 void (*callback) (void*, AudioPluginInstance*, const String&))
{
  AudioPluginInstance* retval = nullptr;
  
  if (desc.name == audioOutDesc.name)
    retval = new AudioProcessorGraph::AudioGraphIOProcessor (AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode);
  
  if (desc.name == audioInDesc.name)
    retval = new AudioProcessorGraph::AudioGraphIOProcessor (AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode);
  
  if (desc.name == midiInDesc.name)
    retval = new AudioProcessorGraph::AudioGraphIOProcessor (AudioProcessorGraph::AudioGraphIOProcessor::midiInputNode);
  
  if (desc.name == midiOutDesc.name)
    retval = new AudioProcessorGraph::AudioGraphIOProcessor (AudioProcessorGraph::AudioGraphIOProcessor::midiOutputNode);
  
  callback (userData, retval, retval == nullptr ? NEEDS_TRANS ("Invalid internal node name") : String());
}

const PluginDescription* InternalPluginFormat::getDescriptionFor (const InternalNodeType type)
{
  switch (type)
  {
    case audioInputNode:      return &audioInDesc;
    case audioOutputNode:     return &audioOutDesc;
    case midiInputNode:       return &midiInDesc;
    case midiOutputNode:      return &midiOutDesc;
    default:                    break;
  }

  return 0;
}

void InternalPluginFormat::getAllTypes (OwnedArray <PluginDescription>& results)
{
  for (int i = 0; i < (int) endOfNodeTypes; ++i)
    results.add (new PluginDescription (*getDescriptionFor ((InternalNodeType) i)));
}


bool InternalPluginFormat::isInternalFormat(String name)
{
  if (name == "Audio Input") return true;
  if (name == "Audio Output") return true;
  if (name == "Midi Input") return true;
  if (name == "Midi Output") return true;
  return false;
}