#include "PluginProcessor.h"
#include "PluginEditor.h"

PMixPluginAudioProcessorEditor::PMixPluginAudioProcessorEditor (PMixPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
  mainComponent = new MainComponent (processor.getAudioEngine());
  addAndMakeVisible(mainComponent);

  addAndMakeVisible (resizer = new ResizableCornerComponent (this, &resizeLimits));
  resizeLimits.setSizeLimits (500, 300, 1920, 1200);
  
  setSize (1200, 800);
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
  resizer->setBounds (getWidth() - 16, getHeight() - 16, 16, 16);
}
