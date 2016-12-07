/*
  ==============================================================================
 
    pMixRHPTabContainer.cpp
    Author:  Oliver Larkin
 
  ==============================================================================
 */

 
#include "pMixRHPTabContainer.h"
#include "pMixMainComponent.h"

PMixTabContainer::PMixTabContainer (PMixAudioEngine& audioEngine, GraphEditor& graphEditor, CodeEditor& codeEditor, InterpolationSpace& iSpace)
: iSpace(iSpace)
, codeEditor(codeEditor)
{
  addAndMakeVisible (tabbedComponent = new TabbedComponent (TabbedButtonBar::TabsAtTop));
  tabbedComponent->setTabBarDepth (23);
  tabbedComponent->addTab (TRANS("Code Editor"), Colours::lightgrey, &codeEditor, false);
  tabbedComponent->addTab (TRANS("Interpolation Space"), Colours::lightgrey, &iSpace, false);
  tabbedComponent->addTab (TRANS("Documentation"), Colours::lightgrey, &docsBrowser, false);
  
  tabbedComponent->setCurrentTabIndex (0);

  tabbedComponent->setOutline(0.);
  
  docsBrowser.goToURL("http://faust.grame.fr/library.html");

  setSize (600, 400);
}

PMixTabContainer::~PMixTabContainer()
{
  tabbedComponent = nullptr;
}

void PMixTabContainer::paint (Graphics& g)
{
  g.fillAll (Colours::white);
}

void PMixTabContainer::resized()
{
  tabbedComponent->setBounds (0, 0, getWidth() - 0, getHeight() - 0);
}

void PMixTabContainer::addTabForContent(Component& content, int window)
{
  switch (window) {
    case MainComponent::kWindowCodeEditor:
      tabbedComponent->addTab (TRANS("Code Editor"), Colours::lightgrey, &codeEditor, false);
      break;
    case MainComponent::kWindowISpace:
      tabbedComponent->addTab (TRANS("Interpolation Space"), Colours::lightgrey, &content, false);
      break;
    case MainComponent::kWindowConsole:
      break;
    default:
      break;
  }
}

void PMixTabContainer::removeTabForContent(Component& content)
{
  for (int i=0; i<tabbedComponent->getNumTabs(); i++) {
    if (tabbedComponent->getTabContentComponent(i) == &content) {
      tabbedComponent->removeTab(i);
    }
  }
  tabbedComponent->setCurrentTabIndex(0);
}
