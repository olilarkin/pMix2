/*
  ==============================================================================

    pMixGenericEditor.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#ifndef PMIXGENERICEDITOR_H_INCLUDED
#define PMIXGENERICEDITOR_H_INCLUDED

#include "JuceHeader.h"

class PMixParamSlider  : public Slider
{
public:
  PMixParamSlider (AudioProcessor& p, int paramIndex);
  
  void valueChanged() override;
  String getTextFromValue (double value) override;
  
private:
  AudioProcessor& owner;
  const int index;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PMixParamSlider)
};

class PMixProcessorParameterPropertyComp : public PropertyComponent
                                         , private AudioProcessorListener
                                         , private Timer
{
public:
  PMixProcessorParameterPropertyComp (const String& name, AudioProcessor& p, int paramIndex);
  ~PMixProcessorParameterPropertyComp();
  
  void refresh() override;
  
  void audioProcessorChanged (AudioProcessor*) override  {}
  
  void audioProcessorParameterChanged (AudioProcessor*, int parameterIndex, float);
  void timerCallback() override;

private:
  AudioProcessor& owner;
  const int index;
  bool volatile paramHasChanged;
  PMixParamSlider slider;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PMixProcessorParameterPropertyComp)
};

class PMixGenericAudioProcessorEditor : public AudioProcessorEditor
{
public:
  PMixGenericAudioProcessorEditor (AudioProcessor* owner);
  ~PMixGenericAudioProcessorEditor();
  
  void paint (Graphics&) override;
  void resized() override;
  
  int getContentHeight() { return panel.getTotalContentHeight(); }
  
private:
  PropertyPanel panel;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PMixGenericAudioProcessorEditor)
};


#endif  // PMIXGENERICEDITOR_H_INCLUDED
