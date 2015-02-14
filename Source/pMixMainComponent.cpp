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

  verticalLayout.setItemLayout (0, -0.2, -0.8, -0.35); // width of the ... must be between 20% and 80%, preferably 50%
  verticalLayout.setItemLayout (1, 8, 8, 8);           // the vertical divider drag-bar thing is always 8 pixels wide
  verticalLayout.setItemLayout (2, 150, -1.0, -0.65);  // the components on the right must be at least 150 pixels wide, preferably 50% of the total width
  verticalDividerBar = new StretchableLayoutResizerBar (&verticalLayout, 1, true);
  addAndMakeVisible (verticalDividerBar);
  
  Logger::setCurrentLogger(&logger);

  addAndMakeVisible (graphEditor = new GraphEditor(audioEngine));
  addAndMakeVisible (paramTreeView = new ParamTreeView(audioEngine));

  interpolationSpace = new InterpolationSpaceComponent(audioEngine);
  
  console = new Console();
  logger.addChangeListener(console);
  fileBrowser = new FileBrowser();
  webBrowser = new WebBrowser(audioEngine);
  paramView = new ParamView(audioEngine);
  codeEditor = new CodeEditor(audioEngine, *webBrowser, *console, *graphEditor);
  graphEditor->addChangeListener(codeEditor);
  graphEditor->addChangeListener(webBrowser);
  
  addAndMakeVisible(hsplitComponent = new SplitComponent(*webBrowser, *console, true));
  addAndMakeVisible(vsplitComponent = new SplitComponent(*codeEditor, *hsplitComponent, false));

  graphEditor->updateComponents();
}

MainComponent::~MainComponent()
{
  LookAndFeel::setDefaultLookAndFeel (nullptr);

  //graphEditor->removeAllChangeListeners();
  Logger::setCurrentLogger (nullptr);

  removeAllChildren();
}

void MainComponent::resized()
{
  if (!vsplitComponent->GetSplitBarPosition())
    vsplitComponent->SetSplitBarPosition(getHeight() / 2.);

  
  Component* vcomps[] = { graphEditor, verticalDividerBar, vsplitComponent };
  
  verticalLayout.layOutComponents (vcomps, 3,
                                   0, 0, getWidth(), getHeight(),
                                   false,     // lay out side-by-side
                                   true);     // resize the components' heights as well as widths
  
  
  if (!hsplitComponent->GetSplitBarPosition())
    hsplitComponent->SetSplitBarPosition(hsplitComponent->getWidth() / 2.);
  
//  graphEditor->setBounds(getBounds());
//  layoutEditor->setTargetComponent(graphEditor);
}

void MainComponent::createNewPlugin (const PluginDescription* desc, int x, int y)
{
  graphEditor->createNewFilter (desc, x, y);
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