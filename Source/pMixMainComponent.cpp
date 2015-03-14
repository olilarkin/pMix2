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
  
  //Logger::setCurrentLogger(&logger);

  addAndMakeVisible (graphEditor = new GraphEditor(audioEngine));
  //addAndMakeVisible (paramTreeView = new ParamTreeView(audioEngine));
  //addAndMakeVisible (paramView = new ParamView(audioEngine));

  addAndMakeVisible(interpolationSpace = new InterpolationSpace(audioEngine));

  addAndMakeVisible( fileBrowser = new FileBrowser());
  addAndMakeVisible( codeEditor = new CodeEditor(audioEngine, *graphEditor) );
  graphEditor->addChangeListener(codeEditor);
  
  graphEditor->updateComponents();
}

MainComponent::~MainComponent()
{
  LookAndFeel::setDefaultLookAndFeel (nullptr);

  //graphEditor->removeAllChangeListeners();
  //Logger::setCurrentLogger (nullptr);

  removeAllChildren();
}

void MainComponent::resized()
{
  Component* hcomps[] = { graphEditor, horizontalDividerBar, codeEditor  };
  
  horizontalLayout.layOutComponents (hcomps, 3, 0, 0, getWidth(), getHeight()-20, false, true);
}

void MainComponent::setZoom (double scale)
{
 // scale = jlimit (1.0 / 4.0, 32.0, scale);
  
  //  if (EditingPanelBase* panel = dynamic_cast <EditingPanelBase*> (tabbedComponent.getCurrentContentComponent()))
  //    panel->setZoom (scale);
}

double MainComponent::getZoom() const
{
  //  if (EditingPanelBase* panel = dynamic_cast <EditingPanelBase*> (tabbedComponent.getCurrentContentComponent()))
  //    return panel->getZoom();
  
  return 1.0;
}