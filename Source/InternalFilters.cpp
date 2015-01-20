#include "../JuceLibraryCode/JuceHeader.h"
#include "InternalFilters.h"
#include "PMixDocument.h"
#include "FaustAudioProcessor.h"

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
    //TODO:, probably the wrong place for this
    FaustAudioProcessor p;
    p.fillInPluginDescription (faustDesc);
  }
}

AudioPluginInstance* InternalPluginFormat::createInstanceFromDescription (const PluginDescription& desc,
    double /*sampleRate*/, int /*blockSize*/)
{
  if (desc.name == audioOutDesc.name)
    return new AudioProcessorGraph::AudioGraphIOProcessor (AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode);

  if (desc.name == audioInDesc.name)
    return new AudioProcessorGraph::AudioGraphIOProcessor (AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode);

  if (desc.name == midiInDesc.name)
    return new AudioProcessorGraph::AudioGraphIOProcessor (AudioProcessorGraph::AudioGraphIOProcessor::midiInputNode);

  if (desc.name == midiOutDesc.name)
    return new AudioProcessorGraph::AudioGraphIOProcessor (AudioProcessorGraph::AudioGraphIOProcessor::midiOutputNode);
  
  if (desc.name == faustDesc.name)
    return new FaustAudioProcessor();

  return 0;
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
