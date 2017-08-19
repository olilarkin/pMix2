/*
  ==============================================================================

    pMixSVGDisplay.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "pMixAudioEngine.h"
#include "pMixGraphEditor.h"

class SVGDisplay : public Component
                 , public ChangeListener
//, public Button::Listener
{
public:
  SVGDisplay(PMixAudioEngine& audioEngine, GraphEditor& graphEditor);
  ~SVGDisplay();
  void paint (Graphics& g) override;
  void resized() override;
  
  void changeListenerCallback (ChangeBroadcaster* source) override;
  //void buttonClicked (Button* button);
  
  void clearDisplay();
  void loadSVG(const String& url);
  
private:
  PMixAudioEngine& audioEngine;
  GraphEditor& graphEditor;
  DrawableComposite* svgDrawable; // used if no webbrowser component
  ScopedPointer<WebBrowserComponent> browser;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SVGDisplay);
};


