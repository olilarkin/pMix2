/*
  ==============================================================================

    FaustAudioProcessor.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "FaustAudioProcessor.h"
#include "FaustAudioProcessorParameter.h"

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
  if (!fDSP)
    create_dsp();
  
  fDSP->init(sampleRate);
  setPlayConfigDetails(fDSP->getNumInputs(),  fDSP->getNumOutputs(), sampleRate, samplesPerBlock);
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
  if(fJSONInterface["name"].isString())
  {
    return fJSONInterface["name"].toString();
  }
  else
    return "Faust Effect";
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

void FaustAudioProcessor::setCurrentProgram (int index)
{
}

const String FaustAudioProcessor::getProgramName (int index)
{
  return "Default";
}

void FaustAudioProcessor::changeProgramName (int index, const String& newName)
{
}

void FaustAudioProcessor::getStateInformation (MemoryBlock& destData)
{
  XmlElement xml ("FAUSTGEN");
  fDSPfactory->getStateInformation(xml);
  copyXmlToBinary (xml, destData);
}

void FaustAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
  ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
  
  if (xmlState != nullptr)
  {
    if (xmlState->hasTagName ("FAUSTGEN"))
    {
      fDSPfactory->setStateInformation(*xmlState);
    }
  }
  
  if (!fDSP)
    create_dsp();
}

void FaustAudioProcessor::create_dsp()
{
  const ScopedLock lock(fDSPfactory->fDSPMutex);

  fDSP = fDSPfactory->create_dsp_aux(this);
  jassert(fDSP);
  
  // Initialize User Interface
  fDSP->buildUserInterface(this);

  Result res = JSON::parse(fDSPfactory->get_json(), fJSONInterface);
  
  // Initialize at the system's sampling rate
  if (getSampleRate() == 0)
    fDSP->init(44100.);
  else
    fDSP->init(getSampleRate());
  
  updateHostDisplay();
}

void FaustAudioProcessor::free_dsp()
{
  deleteDSPInstance(fDSP);
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
  create_dsp();

  // state is modified...
  //set_dirty();
}

String FaustAudioProcessor::get_sourcecode()
{
  return fDSPfactory->get_sourcecode();
}

void FaustAudioProcessor::hilight_on(const String& error)
{
  //TODO:hilight_on
}

void FaustAudioProcessor::hilight_off()
{
  //TODO:hilight_off
}

void FaustAudioProcessor::openTabBox(const char* label)
{
  //TODO:
}

void FaustAudioProcessor::openHorizontalBox(const char* label)
{
  //TODO:
}

void FaustAudioProcessor::openVerticalBox(const char* label)
{
  //TODO:
}

void FaustAudioProcessor::closeBox()
{
  //TODO:
}

void FaustAudioProcessor::addButton(const char* label, FAUSTFLOAT* zone)
{
  //TODO:
}

void FaustAudioProcessor::addCheckButton(const char* label, FAUSTFLOAT* zone)
{
  //TODO:
}

void FaustAudioProcessor::addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
{
  addParameter(new FaustAudioProcessorParameter(String(label), FaustAudioProcessorParameter::kTypeFloat, init, min, max, step, "", zone));
}

void FaustAudioProcessor::addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
{
  addParameter(new FaustAudioProcessorParameter(String(label), FaustAudioProcessorParameter::kTypeFloat, init, min, max, step, "", zone));
}

void FaustAudioProcessor::addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
{
  addParameter(new FaustAudioProcessorParameter(String(label), FaustAudioProcessorParameter::kTypeFloat, init, min, max, step, "", zone));
}

void FaustAudioProcessor::addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
{
  //TODO:
}

void FaustAudioProcessor::addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
{
  //TODO:
}
