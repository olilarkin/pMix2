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

  verticalLayout.setItemLayout (0, -0.2, -0.8, -0.35); // width of the font list must be between 20% and 80%, preferably 50%
  verticalLayout.setItemLayout (1, 8, 8, 8);           // the vertical divider drag-bar thing is always 8 pixels wide
  verticalLayout.setItemLayout (2, 150, -1.0, -0.65);  // the components on the right must be at least 150 pixels wide, preferably 50% of the total width
  verticalDividerBar = new StretchableLayoutResizerBar (&verticalLayout, 1, true);
  addAndMakeVisible (verticalDividerBar);
  
  addAndMakeVisible (graphEditor = new GraphEditor (audioEngine));
  //  addAndMakeVisible (treeView = new ParamTreeView(graph));
    
  //keyState.addListener (&audio.getGraphPlayer().getMidiMessageCollector());
  
  //addAndMakeVisible (keyboardComp = new MidiKeyboardComponent (keyState, MidiKeyboardComponent::horizontalKeyboard));
  
  addAndMakeVisible (interpolationSpace = new InterpolationSpaceComponent(audioEngine));
  addAndMakeVisible (paramView = new ParamView(audioEngine));
  addAndMakeVisible (codeEditor = new CodeEditor());
  addAndMakeVisible (statusBar = new TooltipBar());
    
  graphEditor->updateComponents();
}

MainComponent::~MainComponent()
{
  LookAndFeel::setDefaultLookAndFeel (nullptr);

  deleteAllChildren();
  
  //keyState.removeListener (&audio.getGraphPlayer().getMidiMessageCollector());
}

void MainComponent::resized()
{
  Component* vcomps[] = { graphEditor, verticalDividerBar, interpolationSpace };
  
  verticalLayout.layOutComponents (vcomps, 3,
                                   0, 0, getWidth(), getHeight(),
                                   false,     // lay out side-by-side
                                   true);     // resize the components' heights as well as widths
}

void MainComponent::createNewPlugin (const PluginDescription* desc, int x, int y)
{
  graphEditor->createNewPlugin (desc, x, y);
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