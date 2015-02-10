/*
  ==============================================================================

    pMixWebBrowser.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#ifndef PMIXWEBBROWSER_H_INCLUDED
#define PMIXWEBBROWSER_H_INCLUDED

#include "JuceHeader.h"
#include "pMixAudioEngine.h"
#include "FaustAudioProcessor.h"

class WebBrowser : public Component
                 , public ChangeListener
//, public Button::Listener
{
public:
  WebBrowser(PMixAudioEngine& audioEngine);
  
  void paint (Graphics& g) override;
  void resized() override;
  
  void changeListenerCallback (ChangeBroadcaster* source);
  //void buttonClicked (Button* button);
  
  ScopedPointer<WebBrowserComponent> browser;

private:
  PMixAudioEngine& audioEngine;
  FaustAudioProcessor* selectedFaustAudioProcessor;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WebBrowser);
};


#endif  // PMIXWEBBROWSER_H_INCLUDED
