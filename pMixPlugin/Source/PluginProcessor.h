#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "pMixAudioEngine.h"

class PMixPluginAudioProcessor  : public AudioProcessor
{
public:

  PMixPluginAudioProcessor();
  ~PMixPluginAudioProcessor();
  
  const String getName() const override { return JucePlugin_Name; }
  bool acceptsMidi() const override { return true; }
  bool producesMidi() const override { return true; }
  double getTailLengthSeconds() const override { return 0.0; }
  int getNumPrograms() override { return 1; }
  int getCurrentProgram() override { return 0; }
  void setCurrentProgram (int index) override {}
  const String getProgramName (int index) override { return String(); }
  void changeProgramName (int index, const String& newName) override {}
  bool hasEditor() const override { return true; }

  void prepareToPlay (double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

  void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

  AudioProcessorEditor* createEditor() override;

//  int getNumParameters() override;
//  float getParameter (int index) override;
//  void setParameter (int index, float newValue) override;

  void getStateInformation (MemoryBlock& destData) override;
  void setStateInformation (const void* data, int sizeInBytes) override;
  
  PMixAudioEngine& getAudioEngine() { return audioEngine; }
  
private:
  PMixAudioEngine audioEngine;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PMixPluginAudioProcessor)
};
