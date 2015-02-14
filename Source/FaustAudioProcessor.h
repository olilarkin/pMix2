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

//#define DEFAULT_SOURCE_CODE "import(\"math.lib\"); \nimport(\"maxmsp.lib\"); \nimport(\"music.lib\"); \nimport(\"oscillator.lib\"); \nimport(\"reduce.lib\"); \nimport(\"filter.lib\"); \nimport(\"effect.lib\"); \n \nprocess=_,_;"
#define FAUSTGEN_VERSION "0.99b"
#define FAUST_PDF_DOCUMENTATION "faust-quick-reference.pdf"

#if JUCE_MAC
  //#include "bench-llvm.cpp"
  #define FAUST_LIBRARY_PATH "/Contents/Resources/"
  #define FAUST_DRAW_PATH "/var/tmp/"
  #define SEPARATOR '/'
#elif JUCE_WIN32
  #define FAUST_LIBRARY_PATH "\\faustgen-resources\\"
  #define FAUST_DRAW_PATH "\\faustgen-resources\\"
  #define SEPARATOR '\\'
#endif

#define LLVM_OPTIMIZATION 3
#define DEFAULT_CODE "process = _;"

#define LOG Logger::getCurrentLogger()->writeToLog

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
  void create_dsp();
  void free_dsp();
  bool allocate_factory(const string& effect_name);
  void update_sourcecode();
  String get_sourcecode();
  faustgen_factory* getFactory() { return fDSPfactory; }
  llvm_dsp* getDSP() { return fDSP; }
  void hilight_on(const String& error);
  void hilight_off();
  
private:
  void createParameters();
  
  faustgen_factory* fDSPfactory;
  llvm_dsp* fDSP;
  var fInterface;
    
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FaustAudioProcessor)
};


#endif  // FAUSTAUDIOPROCESSOR_H_INCLUDED
