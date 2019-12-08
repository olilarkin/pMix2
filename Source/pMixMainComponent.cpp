/*
  ==============================================================================

    pMixMainComponent.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixMainComponent.h"

MainComponent::MainComponent (PMixAudioEngine& audioEngine)
: audioEngine(audioEngine)
, iSpaceWindow(nullptr)
, codeEditorWindow(nullptr)
{
  //setOpenGLRenderingEngine();
  LookAndFeel::setDefaultLookAndFeel (&lookAndFeel);

  horizontalLayout.setItemLayout (0, -0.2, -0.8, -0.35);
  horizontalLayout.setItemLayout (1, 8, 8, 8);
  horizontalLayout.setItemLayout (2, 150, -1.0, -0.65);
  
  horizontalDividerBar = std::make_unique<StretchableLayoutResizerBar>(&horizontalLayout, 1, true);
  addAndMakeVisible (*horizontalDividerBar);
  
  graphEditor = std::make_unique<GraphEditor>(audioEngine);
  addAndMakeVisible (*graphEditor);
  
  codeEditor = std::make_unique<CodeEditor>(audioEngine, *graphEditor);
  iSpace = std::make_unique<InterpolationSpace>(audioEngine, *graphEditor);
  
  rightHandPanel = std::make_unique<PMixTabContainer>(audioEngine, *graphEditor, *codeEditor, *iSpace);
  addAndMakeVisible(*rightHandPanel);
  
  graphEditor->updateComponents();
  
//  if (audioEngine.getAppProperties().getUserSettings()->getBoolValue("floatInterpolationSpace")) {
//    floatWindow(true);
//  }
}

MainComponent::~MainComponent()
{
  LookAndFeel::setDefaultLookAndFeel (nullptr);
//  logger.removeChangeListener(codeEditor->console);
  Logger::setCurrentLogger (nullptr);
  
  if(iSpaceWindow != nullptr)
    delete iSpaceWindow;
  
  if(codeEditorWindow != nullptr)
    delete codeEditorWindow;

  removeAllChildren();
}

void MainComponent::paint (Graphics& g)
{
  g.fillAll (Colours::white);
}

void MainComponent::resized()
{
  Component* hcomps[] = { graphEditor.get(), horizontalDividerBar.get(), rightHandPanel.get()  };
  
  horizontalLayout.layOutComponents (hcomps, 3, 0, 0, getWidth(), getHeight(), false, true);
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

void MainComponent::floatWindow(int whichWindow, bool floatIt)
{
  if (whichWindow == kWindowCodeEditor )
  {
    bool isFloating = !(codeEditorWindow == nullptr);
    
    if (isFloating) // Allready floating
    {
      if (floatIt)
      {
        return;
      }
      else // close floating window, move content to tab and update prefs
      {
        delete codeEditorWindow;
        codeEditorWindow = nullptr;
        rightHandPanel->addTabForContent(*codeEditor, kWindowCodeEditor);
        audioEngine.getAppProperties().getUserSettings()->setValue("floatCodeEditor", false);
      }
    }
    else // not yet floating: create window, move content and update prefs
    {
      codeEditorWindow = new PMixFloatWindow("Code Editor", this, kWindowCodeEditor);
      rightHandPanel->removeTabForContent(*codeEditor);
      codeEditorWindow->attachContent(codeEditor.get());
      codeEditorWindow->setVisible(true);
      
      audioEngine.getAppProperties().getUserSettings()->setValue("floatCodeEditor", true);
    }
  }
  else if (whichWindow == kWindowISpace)
  {
    bool isFloating = !(iSpaceWindow == nullptr);
    
    if (isFloating) // Allready floating
    {
      if (floatIt)
      {
        return;
      }
      else // close floating window, move content to tab and update prefs
      {
        delete iSpaceWindow;
        iSpaceWindow = nullptr;
        rightHandPanel->addTabForContent(*iSpace, kWindowISpace);
        audioEngine.getAppProperties().getUserSettings()->setValue("floatInterpolationSpace", false);
      }
    }
    else // not yet floating: create window, move content and update prefs
    {
      iSpaceWindow = new PMixFloatWindow("Interpolation Space", this, kWindowISpace);
      rightHandPanel->removeTabForContent(*iSpace);
      iSpaceWindow->attachContent(iSpace.get());
      iSpaceWindow->setVisible(true);
      
      audioEngine.getAppProperties().getUserSettings()->setValue("floatInterpolationSpace", true);
    }
  }

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
  if (getRenderingEngines()[index] == openGLRendererName)
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
