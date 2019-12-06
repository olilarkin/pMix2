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

std::unique_ptr<AudioPluginInstance> InternalPluginFormat::createInstance (const String& name)
{
    if (name == audioOutDesc.name) return std::make_unique<AudioProcessorGraph::AudioGraphIOProcessor> (AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode);
    if (name == audioInDesc.name)  return std::make_unique<AudioProcessorGraph::AudioGraphIOProcessor> (AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode);
    if (name == midiInDesc.name)   return std::make_unique<AudioProcessorGraph::AudioGraphIOProcessor> (AudioProcessorGraph::AudioGraphIOProcessor::midiInputNode);
    if (name == midiOutDesc.name)  return std::make_unique<AudioProcessorGraph::AudioGraphIOProcessor> (AudioProcessorGraph::AudioGraphIOProcessor::midiOutputNode);

    return {};
}

void InternalPluginFormat::createPluginInstance (const PluginDescription& desc, double, int initialBufferSize, PluginCreationCallback callback)
{
  if (auto p = createInstance (desc.name))
      callback (std::move (p), {});
  else
      callback (nullptr, NEEDS_TRANS ("Invalid internal plugin name"));
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
