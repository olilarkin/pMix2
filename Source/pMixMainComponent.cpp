/*
  ==============================================================================

    MainComponent.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixMainComponent.h"

MainComponent::MainComponent (PMixAudioEngine& audioEngine)
: audioEngine(audioEngine)
{
  LookAndFeel::setDefaultLookAndFeel (&lookAndFeel);

  horizontalLayout.setItemLayout (0, -0.2, -0.8, -0.35);
  horizontalLayout.setItemLayout (1, 8, 8, 8);
  horizontalLayout.setItemLayout (2, 150, -1.0, -0.65);
  
  horizontalDividerBar = new StretchableLayoutResizerBar (&horizontalLayout, 1, true);
  addAndMakeVisible (horizontalDividerBar);
  
  addAndMakeVisible (graphEditor = new GraphEditor(audioEngine));
  
  codeEditor = new CodeEditor (audioEngine, *graphEditor);
  iSpace = new InterpolationSpace (audioEngine, *graphEditor);
  
  addAndMakeVisible(rightHandPanel = new PMixTabContainer(audioEngine, *graphEditor, *codeEditor, *iSpace));

  graphEditor->updateComponents();
}

MainComponent::~MainComponent()
{
  LookAndFeel::setDefaultLookAndFeel (nullptr);
//  logger.removeChangeListener(codeEditor->console);
  Logger::setCurrentLogger (nullptr);

  removeAllChildren();
}

void MainComponent::resized()
{
  Component* hcomps[] = { graphEditor, horizontalDividerBar, rightHandPanel  };
  
  horizontalLayout.layOutComponents (hcomps, 3, 0, 0, getWidth(), getHeight()-20, false, true);
}

void MainComponent::setZoom (double scale)
{
}

double MainComponent::getZoom() const
{
  return 1.0;
}

void MainComponent::clear()
{
  graphEditor->clear();
  rightHandPanel->getCodeEditor()->clear();
}