/*
  ==============================================================================

    pMixGenericEditor.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "pMixAudioEngine.h"

class PMixParamSlider : public Slider
{
public:
  PMixParamSlider (PMixAudioEngine &audioEngine, AudioProcessor& p, int paramIdx, uint32 nodeID);
  
  void valueChanged() override;
  String getTextFromValue (double value) override;
  
  Colour getSliderColour();
  
private:
  PMixAudioEngine &audioEngine;
  AudioProcessor& owner;
  const int index;
  const uint32 nodeID;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PMixParamSlider)
};

class PMixProcessorParameterPropertyComp : public PropertyComponent
                                         , private AudioProcessorListener
                                         , private Timer
{
public:
  PMixProcessorParameterPropertyComp (PMixAudioEngine &audioEngine, const String& name, AudioProcessor& p, int paramIdx, uint32 nodeID);
  ~PMixProcessorParameterPropertyComp();
  
  void refresh() override;
  
  void audioProcessorChanged (AudioProcessor*) override  {}
  
  void audioProcessorParameterChanged (AudioProcessor* p, int parameterIndex, float newValue) override;
  void timerCallback() override;
  void mouseDown (const MouseEvent& e) override;
private:
  PMixAudioEngine &audioEngine;
  AudioProcessor& owner;
  const int index;
  bool volatile paramHasChanged;
  PMixParamSlider slider;
  uint32 nodeID;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PMixProcessorParameterPropertyComp)
};

class PMixGenericAudioProcessorEditor : public AudioProcessorEditor
{
public:
  PMixGenericAudioProcessorEditor (PMixAudioEngine &audioEngine, AudioProcessor* owner, uint32 nodeID);
  ~PMixGenericAudioProcessorEditor();
  
  void paint (Graphics&) override;
  void resized() override;
  
  int getContentHeight() { return panel.getTotalContentHeight(); }
  
private:
  PropertyPanel panel;
  PMixAudioEngine &audioEngine;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PMixGenericAudioProcessorEditor)
};


