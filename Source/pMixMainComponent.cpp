/*
  ==============================================================================

    MainComponent.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixMainComponent.h"

MainComponent::MainComponent (PMixAudioEngine& audioEngine)
: audioEngine(audioEngine)
, iSpaceWindow(nullptr)
, codeEditorWindow(nullptr)
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
  Component* hcomps[] = { graphEditor, horizontalDividerBar, rightHandPanel  };
  
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