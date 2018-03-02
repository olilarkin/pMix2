/*
  ==============================================================================

    pMixPluginWindow.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class PluginWindow  : public DocumentWindow
{
public:
  enum WindowFormatType
  {
    Normal = 0,
    Generic,
//    Programs,
    Parameters
  };
  
  PluginWindow (Component* pluginEditor, AudioProcessorGraph::Node*, WindowFormatType);
  ~PluginWindow();
  
  static PluginWindow* getWindowFor (AudioProcessorGraph::Node*, WindowFormatType);
  
  static void closeCurrentlyOpenWindowsFor (const uint32 nodeID);
  static void closeAllCurrentlyOpenWindows();
  
  void moved() override;
  void closeButtonPressed() override;
  
private:
  AudioProcessorGraph::Node* owner;
  WindowFormatType type;
  
  float getDesktopScaleFactor() const override     { return 1.0f; }
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginWindow)
};

