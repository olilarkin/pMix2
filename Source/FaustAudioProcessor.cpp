/*
  ==============================================================================

    FaustAudioProcessor.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "FaustAudioProcessor.h"

FaustAudioProcessor::FaustAudioProcessor()
: fDSPfactory(nullptr)
, fDSP(nullptr)
{
  // Empty (= no name) FaustAudioProcessor will be internally separated as groups with different names
  if (!fDSPfactory)
  {
    String effect_name;
    effect_name << "faustgen_factory-" << faustgen_factory::gFaustCounter;
    allocate_factory(effect_name.toStdString());
  }
  
  create_dsp(true);
}

FaustAudioProcessor::~FaustAudioProcessor()
{
  free_dsp();
  
  fDSPfactory->remove_instance(this);
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
  description.numInputChannels = fDSP->getNumInputs();
  description.numOutputChannels = fDSP->getNumOutputs();
}

//static
void FaustAudioProcessor::fillInitialInPluginDescription (PluginDescription& description)
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
  description.numInputChannels = 1;
  description.numOutputChannels = 1;
}

void FaustAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
  setPlayConfigDetails(fDSP->getNumInputs(),  fDSP->getNumOutputs(), sampleRate, samplesPerBlock);
  fDSP->init(sampleRate);
}

void FaustAudioProcessor::releaseResources()
{
}

void FaustAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
  const ScopedLock lock(fDSPfactory->fDSPMutex);
  
  if (fDSP != nullptr)
    fDSP->compute(buffer.getNumSamples(), (FAUSTFLOAT**)buffer.getArrayOfReadPointers(), (FAUSTFLOAT**)buffer.getArrayOfWritePointers());
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

void FaustAudioProcessor::create_dsp(bool init)
{
  const ScopedLock lock(fDSPfactory->fDSPMutex);

  fDSP = fDSPfactory->create_dsp_aux(this);
  assert(fDSP);
  
  // Initialize User Interface (here connnection with controls)
  //fDSP->buildUserInterface(&fDSPUI);
  
  // Initialize at the system's sampling rate
  fDSP->init(getSampleRate());
  
  //TODO: create Audio IO
  
  updateHostDisplay();
}

void FaustAudioProcessor::free_dsp()
{
  deleteDSPInstance(fDSP);
  //fDSPUI.clear();
  fDSP = 0;
}

bool FaustAudioProcessor::allocate_factory(const string& effect_name)
{
  bool res = false;
  
  if (faustgen_factory::gFactoryMap.find(effect_name) != faustgen_factory::gFactoryMap.end())
  {
    fDSPfactory = faustgen_factory::gFactoryMap[effect_name];
  }
  else
  {
    fDSPfactory = new faustgen_factory(effect_name);
    faustgen_factory::gFactoryMap[effect_name] = fDSPfactory;
    res = true;
  }
  
  fDSPfactory->add_instance(this);
  return res;
}

void FaustAudioProcessor::update_sourcecode()
{
  // Create a new DSP instance
  create_dsp(false);

  // state is modified...
  //set_dirty();
}

String FaustAudioProcessor::get_sourcecode()
{
  return fDSPfactory->get_sourcecode();
}