/*
  ==============================================================================

    FaustProcessor.cpp
    Created: 15 Jan 2015 4:32:44pm
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "FaustProcessor.h"

FaustAudioProcessor::FaustAudioProcessor()
: mFactory(nullptr)
{
//  int argc = 3;
//    const char* argv[argc];
//  argv[0] = "-vec";
//  argv[1] = "-lv";
//  argv[2] = " 1";
//  
  // Faust program
  std::string faust_program = "process = +;";
  
  mFactory = createDSPFactoryFromString("test", faust_program, 0, nullptr, "", mError_msg, 3);
  
  if (mFactory != nullptr)
  {
    printf("success");
  }
}

FaustAudioProcessor::~FaustAudioProcessor()
{
  if (mFactory != nullptr)
  {
    deleteDSPFactory(mFactory);
  }
}

void FaustAudioProcessor::fillInPluginDescription (PluginDescription& description) const
{
  description.name = "FaustProc";
  description.descriptiveName = "Faust Processor";
  description.pluginFormatName = "Faust JIT compiled";
  description.category = "na";
  description.manufacturerName = "bleh";
  description.version = "0.0.1";
  description.fileOrIdentifier = "";
  description.lastFileModTime = Time(0);
  description.isInstrument = false;
  description.hasSharedContainer = false;
  description.numInputChannels = 2;
  description.numOutputChannels = 2;
}

void FaustAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
  
}

void FaustAudioProcessor::releaseResources()
{
  
}

void FaustAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
  
}

void FaustAudioProcessor::reset()
{
  
}

bool FaustAudioProcessor::hasEditor() const
{
  return false;
}

AudioProcessorEditor* FaustAudioProcessor::createEditor()
{
  return nullptr;
}

const String FaustAudioProcessor::getName() const
{
  return "FaustAudioProcessor";
}

int FaustAudioProcessor::getNumParameters()
{
  return 1;
}

float FaustAudioProcessor::getParameter (int index)
{
  return 0.;
}

float FaustAudioProcessor::getParameterDefaultValue (int index)
{
  return 0.;
}

void FaustAudioProcessor::setParameter (int index, float newValue)
{
  
}

const String FaustAudioProcessor::getParameterName (int index)
{
  return "unknown";
}

const String FaustAudioProcessor::getParameterText (int index)
{
  return "unknown";
}

const String FaustAudioProcessor::getInputChannelName (int channelIndex) const
{
  return "unknown";
}

const String FaustAudioProcessor::getOutputChannelName (int channelIndex) const
{
  return "unknown";
}

bool FaustAudioProcessor::isInputChannelStereoPair (int index) const
{
  return true;
}

bool FaustAudioProcessor::isOutputChannelStereoPair (int index) const
{
  return true;
}


bool FaustAudioProcessor::acceptsMidi() const
{
  return false;
}

bool FaustAudioProcessor::producesMidi() const
{
  return false;
}

bool FaustAudioProcessor::silenceInProducesSilenceOut() const
{
  return true;
}

double FaustAudioProcessor::getTailLengthSeconds() const
{
  return 0.;
}

int FaustAudioProcessor::getNumPrograms()
{
  return 1;
}

int FaustAudioProcessor::getCurrentProgram()
{
  return 0;
}

void FaustAudioProcessor::setCurrentProgram (int /*index*/)
{
}

const String FaustAudioProcessor::getProgramName (int /*index*/)
{
  return "Default";
}

void FaustAudioProcessor::changeProgramName (int /*index*/, const String& /*newName*/)
{
}

void FaustAudioProcessor::getStateInformation (MemoryBlock& destData)
{
}

void FaustAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}
