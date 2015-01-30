#include "PluginProcessor.h"
#include "PluginEditor.h"

PMixPluginAudioProcessor::PMixPluginAudioProcessor()
{
}

PMixPluginAudioProcessor::~PMixPluginAudioProcessor()
{
}

const String PMixPluginAudioProcessor::getName() const
{
  return JucePlugin_Name;
}

int PMixPluginAudioProcessor::getNumParameters()
{
  return 0;
}

float PMixPluginAudioProcessor::getParameter (int index)
{
  return 0.0f;
}

void PMixPluginAudioProcessor::setParameter (int index, float newValue)
{
}

const String PMixPluginAudioProcessor::getParameterName (int index)
{
  return String();
}

const String PMixPluginAudioProcessor::getParameterText (int index)
{
  return String();
}

const String PMixPluginAudioProcessor::getInputChannelName (int channelIndex) const
{
  return String (channelIndex + 1);
}

const String PMixPluginAudioProcessor::getOutputChannelName (int channelIndex) const
{
  return String (channelIndex + 1);
}

bool PMixPluginAudioProcessor::isInputChannelStereoPair (int index) const
{
  return true;
}

bool PMixPluginAudioProcessor::isOutputChannelStereoPair (int index) const
{
  return true;
}

bool PMixPluginAudioProcessor::acceptsMidi() const
{
 #if JucePlugin_WantsMidiInput
  return true;
 #else
  return false;
 #endif
}

bool PMixPluginAudioProcessor::producesMidi() const
{
 #if JucePlugin_ProducesMidiOutput
  return true;
 #else
  return false;
 #endif
}

bool PMixPluginAudioProcessor::silenceInProducesSilenceOut() const
{
  return false;
}

double PMixPluginAudioProcessor::getTailLengthSeconds() const
{
  return 0.0;
}

int PMixPluginAudioProcessor::getNumPrograms()
{
  return 1;
}

int PMixPluginAudioProcessor::getCurrentProgram()
{
  return 0;
}

void PMixPluginAudioProcessor::setCurrentProgram (int index)
{
}

const String PMixPluginAudioProcessor::getProgramName (int index)
{
  return String();
}

void PMixPluginAudioProcessor::changeProgramName (int index, const String& newName)
{
}

void PMixPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
  audio.getGraph().setPlayConfigDetails (getNumInputChannels(), getNumOutputChannels(), sampleRate, samplesPerBlock);
  audio.getGraph().prepareToPlay (sampleRate, samplesPerBlock);

//  audio.getGraph().addConnection (myProcessor->getNodeID(), 0, ioProcOutNode->id, 0);
//  audio.getGraph().addConnection (myProcessor->getNodeID(), 1, ioProcOutNode->id, 1);
}

void PMixPluginAudioProcessor::releaseResources()
{
audio.getGraph().releaseResources();
}

void PMixPluginAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
  audio.getGraph().processBlock (buffer, midiMessages);
}

bool PMixPluginAudioProcessor::hasEditor() const
{
  return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* PMixPluginAudioProcessor::createEditor()
{
  return new PMixPluginAudioProcessorEditor (*this);
}


void PMixPluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
}

void PMixPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
  return new PMixPluginAudioProcessor();
}
