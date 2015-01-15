/*
  ==============================================================================

    FaustProcessor.h
    Created: 15 Jan 2015 4:32:44pm
    Author:  Oliver Larkin

  ==============================================================================
*/

#ifndef FAUSTPROCESSOR_H_INCLUDED
#define FAUSTPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "/usr/local/include/faust/llvm-dsp.h"

class FaustAudioProcessor  : public AudioPluginInstance
{
public:
  FaustAudioProcessor();
  ~FaustAudioProcessor();
  
  virtual void fillInPluginDescription (PluginDescription& description) const override;
  
  void prepareToPlay (double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;
  void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override;
  void reset() override;
  
  bool hasEditor() const override                  { return false; }
  AudioProcessorEditor* createEditor() override;
  
  const String getName() const override            { return "FaustAudioProcessor"; }
  
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
  int getCurrentProgram() override                                            { return 0; }
  void setCurrentProgram (int /*index*/) override                             {}
  const String getProgramName (int /*index*/) override                        { return "Default"; }
  void changeProgramName (int /*index*/, const String& /*newName*/) override  {}
  
  void getStateInformation (MemoryBlock& destData) override;
  void setStateInformation (const void* data, int sizeInBytes) override;
  
private:
  llvm_dsp_factory* mFactory;
  std::string mError_msg;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FaustAudioProcessor)
};


#endif  // FAUSTPROCESSOR_H_INCLUDED
