#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "pMixMainComponent.h"

class PMixPluginAudioProcessorEditor  : public AudioProcessorEditor
{
public:
  PMixPluginAudioProcessorEditor (PMixPluginAudioProcessor&);
  ~PMixPluginAudioProcessorEditor();

  void paint (Graphics&) override;
  void resized() override;

private:
  PMixPluginAudioProcessor& processor;
  ScopedPointer<MainComponent> mainComponent;
  ScopedPointer<ResizableCornerComponent> resizer;
  ComponentBoundsConstrainer resizeLimits;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PMixPluginAudioProcessorEditor)
};