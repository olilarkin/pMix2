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

#include "faust/llvm-dsp.h"

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
  void setCurrentProgram (int /*index*/) override;
  const String getProgramName (int /*index*/) override;
  void changeProgramName (int /*index*/, const String& /*newName*/) override;
  
  void getStateInformation (MemoryBlock& destData) override;
  void setStateInformation (const void* data, int sizeInBytes) override;
  
private:
  llvm_dsp_factory* mFactory;
  dsp* mDSP;
  
  std::string mError_msg;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FaustAudioProcessor)
};


#endif  // FAUSTAUDIOPROCESSOR_H_INCLUDED
