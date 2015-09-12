/*
  ==============================================================================

    pMixFloatWindow.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#ifndef PMIXFLOATWINDOW_H_INCLUDED
#define PMIXFLOATWINDOW_H_INCLUDED

#include "JuceHeader.h"

class PMixFloatWindow  : public DocumentWindow
{
public:
  PMixFloatWindow (const String &name, Component &contentComponent)
  : DocumentWindow(name, Colours::white, DocumentWindow::allButtons, true)
  {
    setUsingNativeTitleBar(true);
    centreWithSize (400, 400);
    //setContentNonOwned(contentComponent, true);
  }
  
  ~PMixFloatWindow() {};
  
  void closeButtonPressed()
  {
    delete this;
  }
};

#endif  // PMIXFLOATWINDOW_H_INCLUDED
