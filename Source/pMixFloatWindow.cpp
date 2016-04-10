/*
  ==============================================================================

    pMixFloatWindow.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixFloatWindow.h"
#include "pMixMainComponent.h"

PMixFloatWindow::PMixFloatWindow (const String &name, Component* parent, int windowID)
: DocumentWindow(name, Colours::white, DocumentWindow::allButtons, true)
, parent(parent)
, windowID(windowID)
{
  setUsingNativeTitleBar(true);
  centreWithSize (400, 400);
  setResizable(true, true);
}

PMixFloatWindow::~PMixFloatWindow()
{
};

void PMixFloatWindow::closeButtonPressed()
{
  dynamic_cast<MainComponent*>(parent)->floatWindow(windowID, false);
}

void PMixFloatWindow::attachContent(Component* content)
{
  setContentNonOwned(content, true);
}