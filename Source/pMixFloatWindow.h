/*
  ==============================================================================

    pMixFloatWindow.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#pragma once

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

