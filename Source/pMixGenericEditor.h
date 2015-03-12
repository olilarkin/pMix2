/*
  ==============================================================================

    pMixGenericEditor.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#ifndef PMIXGENERICEDITOR_H_INCLUDED
#define PMIXGENERICEDITOR_H_INCLUDED

#include "JuceHeader.h"
#include "pMixAudioEngine.h"

class PMixParamSlider : public Slider
{
public:
  PMixParamSlider (PMixAudioEngine &audioEngine, AudioProcessor& p, int paramIdx, uint32 filterID);
  
  void valueChanged() override;
  String getTextFromValue (double value) override;
  
  Colour getSliderColour();
  
private:
  PMixAudioEngine &audioEngine;
  AudioProcessor& owner;
  const int index;
  const uint32 filterID;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PMixParamSlider)
};

class PMixProcessorParameterPropertyComp : public PropertyComponent
                                         , private AudioProcessorListener
                                         , private Timer
{
public:
  PMixProcessorParameterPropertyComp (PMixAudioEngine &audioEngine, const String& name, AudioProcessor& p, int paramIdx, uint32 filterID);
  ~PMixProcessorParameterPropertyComp();
  
  void refresh() override;
  
  void audioProcessorChanged (AudioProcessor*) override  {}
  
  void audioProcessorParameterChanged (AudioProcessor* p, int parameterIndex, float newValue);
  void timerCallback();
  void mouseDown (const MouseEvent& e);
private:
  PMixAudioEngine &audioEngine;
  AudioProcessor& owner;
  const int index;
  bool volatile paramHasChanged;
  PMixParamSlider slider;
  uint32 filterID;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PMixProcessorParameterPropertyComp)
};

class PMixGenericAudioProcessorEditor : public AudioProcessorEditor
{
public:
  PMixGenericAudioProcessorEditor (PMixAudioEngine &audioEngine, AudioProcessor* owner, uint32 filterID);
  ~PMixGenericAudioProcessorEditor();
  
  void paint (Graphics&) override;
  void resized() override;
  
  int getContentHeight() { return panel.getTotalContentHeight(); }
  
private:
  PropertyPanel panel;
  PMixAudioEngine &audioEngine;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PMixGenericAudioProcessorEditor)
};


#endif  // PMIXGENERICEDITOR_H_INCLUDED
