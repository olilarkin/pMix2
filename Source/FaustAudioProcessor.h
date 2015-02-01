/*
  ==============================================================================

    FaustAudioProcessor.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#ifndef FAUSTAUDIOPROCESSOR_H_INCLUDED
#define FAUSTAUDIOPROCESSOR_H_INCLUDED

#include "JuceHeader.h"

#define FAUSTFLOAT float

#define DEFAULT_SOURCE_CODE "import(\"math.lib\"); \nimport(\"maxmsp.lib\"); \nimport(\"music.lib\"); \nimport(\"oscillator.lib\"); \nimport(\"reduce.lib\"); \nimport(\"filter.lib\"); \nimport(\"effect.lib\"); \n \nprocess=_,_;"
#define FAUSTGEN_VERSION "0.99b"
#define FAUST_PDF_DOCUMENTATION "faust-quick-reference.pdf"

#ifdef __APPLE__
//#include "bench-llvm.cpp"
#define FAUST_LIBRARY_PATH "/Contents/Resources/"
#define FAUST_DRAW_PATH "/var/tmp/"
#define SEPARATOR '/'
#endif

#ifdef WIN32
#define FAUST_LIBRARY_PATH "\\faustgen-resources\\"
#define FAUST_DRAW_PATH "\\faustgen-resources\\"
#define SEPARATOR '\\'
#endif

#define LLVM_OPTIMIZATION 3
#define DEFAULT_CODE "process = _,*(0.1);"


#include "FaustGenFactory.h"

class FaustAudioProcessor  : public AudioPluginInstance
{
public:
  FaustAudioProcessor();
  ~FaustAudioProcessor();
  
  void fillInPluginDescription (PluginDescription& description) const override;
  static void fillInitialInPluginDescription (PluginDescription& description);

  void prepareToPlay (double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;
  void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override;
  void reset() override;
  
  bool hasEditor() const override;
  AudioProcessorEditor* createEditor() override;
  
  const String getName() const override;
  
  int getNumParameters() override;
  float getParameter (int index) override;
  float getParameterDefaultValue (int index) override;
  void setParameter (int index, float newValue) override;
  const String getParameterName (int index) override;
  const String getParameterText (int index) override;
  
  const String getInputChannelName (int channelIndex) const override;
  const String getOutputChannelName (int channelIndex) const override;
  bool isInputChannelStereoPair (int index) const override;
  bool isOutputChannelStereoPair (int index) const override;
  
  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool silenceInProducesSilenceOut() const override;
  double getTailLengthSeconds() const override;
  
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram (int index) override;
  const String getProgramName (int index) override;
  void changeProgramName (int index, const String& newName) override;
  
  void getStateInformation (MemoryBlock& destData) override;
  void setStateInformation (const void* data, int sizeInBytes) override;
  
//Unique
  void create_dsp(bool init);
  void free_dsp();
  bool allocate_factory(const string& effect_name);

private:
  faustgen_factory* fDSPfactory;
  llvm_dsp* fDSP;
    
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FaustAudioProcessor)
};


#endif  // FAUSTAUDIOPROCESSOR_H_INCLUDED
