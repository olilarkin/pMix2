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
  PMixFloatWindow (const String &name, Component* parent, int windowID);
  ~PMixFloatWindow();
  
  void closeButtonPressed();
  
  void attachContent(Component* content);
private:
  Component *parent;
  int windowID;
};

#endif  // PMIXFLOATWINDOW_H_INCLUDED
