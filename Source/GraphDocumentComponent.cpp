/*
  ==============================================================================

    GraphDocumentComponent.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "GraphDocumentComponent.h"

GraphDocumentComponent::GraphDocumentComponent (AudioPluginFormatManager& formatManager,
                                                AudioDeviceManager* deviceManager_)
: graph (formatManager), deviceManager (deviceManager_)
{
  LookAndFeel::setDefaultLookAndFeel(&lf);
  
  verticalLayout.setItemLayout (0, -0.2, -0.8, -0.35); // width of the font list must be between 20% and 80%, preferably 50%
  verticalLayout.setItemLayout (1, 8, 8, 8);           // the vertical divider drag-bar thing is always 8 pixels wide
  verticalLayout.setItemLayout (2, 150, -1.0, -0.65);  // the components on the right must be at least 150 pixels wide, preferably 50% of the total width
  verticalDividerBar = new StretchableLayoutResizerBar (&verticalLayout, 1, true);
  addAndMakeVisible (verticalDividerBar);
  
  addAndMakeVisible (graphPanel = new GraphEditorPanel (graph, undoManager));
  //  addAndMakeVisible (treeView = new ParamTreeView(graph));
  
  deviceManager->addChangeListener (graphPanel);
  
  graphPlayer.setProcessor (&graph.getGraph());
  
  keyState.addListener (&graphPlayer.getMidiMessageCollector());
  
  addAndMakeVisible (keyboardComp = new MidiKeyboardComponent (keyState, MidiKeyboardComponent::horizontalKeyboard));
  
  addAndMakeVisible (iSpace = new iSpaceComponent(undoManager));
  addAndMakeVisible (paramView = new ParamView(graph));
  addAndMakeVisible (statusBar = new TooltipBar());
  
  deviceManager->addAudioCallback (&graphPlayer);
  deviceManager->addMidiInputCallback (String::empty, &graphPlayer.getMidiMessageCollector());
  
  graphPanel->updateComponents();
}

GraphDocumentComponent::~GraphDocumentComponent()
{
  deviceManager->removeAudioCallback (&graphPlayer);
  deviceManager->removeMidiInputCallback (String::empty, &graphPlayer.getMidiMessageCollector());
  deviceManager->removeChangeListener (graphPanel);
  
  deleteAllChildren();
  
  graphPlayer.setProcessor (nullptr);
  keyState.removeListener (&graphPlayer.getMidiMessageCollector());
  
  graph.clear();
}

void GraphDocumentComponent::resized()
{
  Component* vcomps[] = { graphPanel, verticalDividerBar, paramView };
  
  verticalLayout.layOutComponents (vcomps, 3,
                                   0, 0, getWidth(), getHeight(),
                                   false,     // lay out side-by-side
                                   true);     // resize the components' heights as well as widths
}

void GraphDocumentComponent::createNewPlugin (const PluginDescription* desc, int x, int y)
{
  graphPanel->createNewPlugin (desc, x, y);
}

void GraphDocumentComponent::setZoom (double scale)
{
  scale = jlimit (1.0 / 4.0, 32.0, scale);
  
  //  if (EditingPanelBase* panel = dynamic_cast <EditingPanelBase*> (tabbedComponent.getCurrentContentComponent()))
  //    panel->setZoom (scale);
}

double GraphDocumentComponent::getZoom() const
{
  //  if (EditingPanelBase* panel = dynamic_cast <EditingPanelBase*> (tabbedComponent.getCurrentContentComponent()))
  //    return panel->getZoom();
  
  return 1.0;
}