/*
  ==============================================================================

    FaustAudioProcessor.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "FaustAudioProcessor.h"

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
    mDSP = createDSPInstance(mFactory);

    if (mDSP)
    {
      printf("success");
    }
  }
  
  updateHostDisplay();
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
  description.name = "Faust Effect";
  description.descriptiveName = "JIT compiled Faust Effect";
  description.pluginFormatName = "Faust JIT compiled";
  description.category = "na";
  description.manufacturerName = "bleh";
  description.version = "0.0.1";
  description.fileOrIdentifier = "";
  description.lastFileModTime = Time(0);
  description.isInstrument = false;
  description.hasSharedContainer = false;
  description.numInputChannels = mDSP->getNumInputs();
  description.numOutputChannels = mDSP->getNumOutputs();
}

void FaustAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
  setPlayConfigDetails(mDSP->getNumInputs(),  mDSP->getNumOutputs(), sampleRate, samplesPerBlock);
  mDSP->init(sampleRate);
}

void FaustAudioProcessor::releaseResources()
{
  
}

void FaustAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
  mDSP->compute(buffer.getNumSamples(), (FAUSTFLOAT**)buffer.getArrayOfReadPointers(), (FAUSTFLOAT**)buffer.getArrayOfWritePointers());
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
  return false;
}

bool FaustAudioProcessor::isOutputChannelStereoPair (int index) const
{
  return false;
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
