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
#include "pMixGraphEditor.h"

class WebBrowser : public Component
                 , public ChangeListener
//, public Button::Listener
{
public:
  WebBrowser(PMixAudioEngine& audioEngine, GraphEditor& graphEditor);
  ~WebBrowser();
  void paint (Graphics& g) override;
  void resized() override;
  
  void changeListenerCallback (ChangeBroadcaster* source) override;
  //void buttonClicked (Button* button);
  
  ScopedPointer<WebBrowserComponent> browser;

private:
  PMixAudioEngine& audioEngine;
  GraphEditor& graphEditor;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WebBrowser);
};


#endif  // PMIXWEBBROWSER_H_INCLUDED
