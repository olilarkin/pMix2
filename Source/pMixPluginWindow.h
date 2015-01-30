/*
  ==============================================================================

    pMixPluginWindow.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#ifndef PMIXPLUGINWINDOW_H_INCLUDED
#define PMIXPLUGINWINDOW_H_INCLUDED

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
  
  static void closeCurrentlyOpenWindowsFor (const uint32 nodeId);
  static void closeAllCurrentlyOpenWindows();
  
  void moved() override;
  void closeButtonPressed() override;
  
private:
  AudioProcessorGraph::Node* owner;
  WindowFormatType type;
  
  float getDesktopScaleFactor() const override     { return 1.0f; }
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginWindow)
};

//class ProcessorProgramPropertyComp : public PropertyComponent,
//private AudioProcessorListener
//{
//public:
//  ProcessorProgramPropertyComp (const String& name, AudioProcessor& p, int index_);
//  ~ProcessorProgramPropertyComp();
//  void refresh();
//  void audioProcessorChanged (AudioProcessor*);
//  void audioProcessorParameterChanged (AudioProcessor*, int, float);
//  
//private:
//  AudioProcessor& owner;
//  const int index;
//  
//  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProcessorProgramPropertyComp)
//};
//
//class ProgramAudioProcessorEditor : public AudioProcessorEditor
//{
//public:
//  ProgramAudioProcessorEditor (AudioProcessor* const p);
//  void paint (Graphics& g);
//  void resized();
//  
//private:
//  PropertyPanel panel;
//  
//  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProgramAudioProcessorEditor)
//};

#endif  // PMIXPLUGINWINDOW_H_INCLUDED
