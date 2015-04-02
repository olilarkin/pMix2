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
  //setOpenGLRenderingEngine();
  LookAndFeel::setDefaultLookAndFeel (&lookAndFeel);

  horizontalLayout.setItemLayout (0, -0.2, -0.8, -0.35);
  horizontalLayout.setItemLayout (1, 8, 8, 8);
  horizontalLayout.setItemLayout (2, 150, -1.0, -0.65);
  
  horizontalDividerBar = new StretchableLayoutResizerBar (&horizontalLayout, 1, true);
  addAndMakeVisible (horizontalDividerBar);
  
  //Logger::setCurrentLogger(&logger);

  addAndMakeVisible (graphEditor = new GraphEditor(audioEngine));
  addAndMakeVisible(rightHandPanel = new PMixTabContainer(audioEngine, *graphEditor));

//  logger.addChangeListener(codeEditor->console);
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

static const char* openGLRendererName = "OpenGL Renderer";

StringArray MainComponent::getRenderingEngines() const
{
  StringArray renderingEngines;
  
  if (ComponentPeer* peer = getPeer())
    renderingEngines = peer->getAvailableRenderingEngines();
  
#if JUCE_OPENGL
  renderingEngines.add (openGLRendererName);
#endif
  
  return renderingEngines;
}

void MainComponent::setRenderingEngine (int index)
{
//  showMessageBubble (getRenderingEngines()[index]);
  
#if JUCE_OPENGL
  if (getRenderingEngines()[index] == openGLRendererName
//      && contentComponent != nullptr
//      && (! contentComponent->isShowingOpenGLDemo())
      )
  {
    openGLContext.attachTo (*getTopLevelComponent());
    return;
  }
  
  openGLContext.detach();
#endif
  
  if (ComponentPeer* peer = getPeer())
    peer->setCurrentRenderingEngine (index);
}

void MainComponent::setOpenGLRenderingEngine()
{
  setRenderingEngine (getRenderingEngines().indexOf (openGLRendererName));
}

int MainComponent::getActiveRenderingEngine() const
{
#if JUCE_OPENGL
  if (openGLContext.isAttached())
    return getRenderingEngines().indexOf (openGLRendererName);
#endif
  
  if (ComponentPeer* peer = getPeer())
    return peer->getCurrentRenderingEngine();
  
  return 0;
}