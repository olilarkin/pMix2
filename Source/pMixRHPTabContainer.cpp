#include "pMixRHPTabContainer.h"

PMixTabContainer::PMixTabContainer (PMixAudioEngine& audioEngine, GraphEditor& graphEditor, CodeEditor& codeEditor, InterpolationSpace& iSpace)
{
  addAndMakeVisible (tabbedComponent = new TabbedComponent (TabbedButtonBar::TabsAtTop));
  tabbedComponent->setTabBarDepth (23);
  tabbedComponent->addTab (TRANS("Code Editor"), Colours::lightgrey, &codeEditor, false);
  tabbedComponent->addTab (TRANS("Interpolation Space"), Colours::lightgrey, &iSpace, false);
  tabbedComponent->setCurrentTabIndex (0);

  tabbedComponent->setOutline(0.);

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
