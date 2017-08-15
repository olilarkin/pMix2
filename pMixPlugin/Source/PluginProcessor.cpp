#include "PluginProcessor.h"
#include "PluginEditor.h"

PMixPluginAudioProcessor::PMixPluginAudioProcessor()
{
  audioEngine.getDoc().initialize();
}

PMixPluginAudioProcessor::~PMixPluginAudioProcessor()
{
}

void PMixPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
  audioEngine.getGraph().setPlayConfigDetails (getTotalNumInputChannels(), getTotalNumOutputChannels(), sampleRate, samplesPerBlock);
  audioEngine.getGraph().prepareToPlay (sampleRate, samplesPerBlock);
}

void PMixPluginAudioProcessor::releaseResources()
{
  audioEngine.getGraph().releaseResources();
}

void PMixPluginAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
  audioEngine.getGraph().processBlock (buffer, midiMessages);
}

void PMixPluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
  ScopedPointer<XmlElement> xml = audioEngine.getDoc().createXml();
  copyXmlToBinary(*xml, destData);
}

void PMixPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
  ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
  
  if (xmlState != nullptr)
  {
    audioEngine.getDoc().restoreFromXml(*xmlState);
  }
}

AudioProcessorEditor* PMixPluginAudioProcessor::createEditor()
{
  return new PMixPluginAudioProcessorEditor (*this);
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
  return new PMixPluginAudioProcessor();
}
