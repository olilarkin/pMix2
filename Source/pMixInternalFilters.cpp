#include "pMixInternalFilters.h"

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
  
  {
    FaustAudioPluginInstance::fillInitialInPluginDescription (faustDesc);
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
  
  if (desc.name == faustDesc.name)
    retval = new FaustAudioPluginInstance();
  
  callback (userData, retval, retval == nullptr ? NEEDS_TRANS ("Invalid internal filter name") : String());
}

const PluginDescription* InternalPluginFormat::getDescriptionFor (const InternalFilterType type)
{
  switch (type)
  {
    case audioInputFilter:      return &audioInDesc;
    case audioOutputFilter:     return &audioOutDesc;
    case midiInputFilter:       return &midiInDesc;
    case midiOutputFilter:      return &midiOutDesc;
    case faustEffect:           return &faustDesc;
    default:                    break;
  }

  return 0;
}

void InternalPluginFormat::getAllTypes (OwnedArray <PluginDescription>& results)
{
  for (int i = 0; i < (int) endOfFilterTypes; ++i)
    results.add (new PluginDescription (*getDescriptionFor ((InternalFilterType) i)));
}


bool InternalPluginFormat::isInternalFormat(String name)
{
  if (name == "Audio Input") return true;
  if (name == "Audio Output") return true;
  if (name == "Midi Input") return true;
  if (name == "Midi Output") return true;
 // if (name == faustDesc.name) return true;
  
  return false;
}