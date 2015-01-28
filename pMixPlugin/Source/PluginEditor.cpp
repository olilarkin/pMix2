#include "PluginProcessor.h"
#include "PluginEditor.h"

PMixPluginAudioProcessorEditor::PMixPluginAudioProcessorEditor (PMixPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
  setSize (400, 300);
}

PMixPluginAudioProcessorEditor::~PMixPluginAudioProcessorEditor()
{
}

void PMixPluginAudioProcessorEditor::paint (Graphics& g)
{
  g.fillAll (Colours::white);

  g.setColour (Colours::black);
  g.setFont (15.0f);
  g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void PMixPluginAudioProcessorEditor::resized()
{
  // This is generally where you'll want to lay out the positions of any
  // subcomponents in your editor..
}
