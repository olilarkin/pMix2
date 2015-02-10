#include "PluginProcessor.h"
#include "PluginEditor.h"

PMixPluginAudioProcessorEditor::PMixPluginAudioProcessorEditor (PMixPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
  mainComponent = new MainComponent (processor.getAudioEngine());
  addAndMakeVisible(mainComponent);
  
  setSize (1000, 700);
}

PMixPluginAudioProcessorEditor::~PMixPluginAudioProcessorEditor()
{
  mainComponent = nullptr;
}

void PMixPluginAudioProcessorEditor::paint (Graphics& g)
{
  g.fillAll (Colours::lightgrey);
}

void PMixPluginAudioProcessorEditor::resized()
{
  mainComponent->setBounds (0, 0, getWidth(), getHeight());
}
