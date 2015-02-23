/*
  ==============================================================================

    pMixWebBrowser.cpp
    Created: 1 Feb 2015 9:43:23pm
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixWebBrowser.h"
#include "pMixGraphEditor.h"

WebBrowser::WebBrowser(PMixAudioEngine& audioEngine)
: audioEngine(audioEngine)
, selectedFaustAudioProcessor(nullptr)
{
  addAndMakeVisible (browser = new WebBrowserComponent ());
  
  browser->goToURL("");
}

void WebBrowser::paint (Graphics& g)
{
  //g.fillAll (Colours::lightgrey);
}

void WebBrowser::resized()
{
  Rectangle<int> r (getLocalBounds());
  browser->setBounds (r);
}

void WebBrowser::changeListenerCallback (ChangeBroadcaster* source)
{
  GraphEditor* graphEditor = dynamic_cast<GraphEditor*>(source);
  
  if (graphEditor)
  {
    if(graphEditor->getLassoSelection().getNumSelected() == 1)
    {
      FilterComponent* selectedItem = dynamic_cast<FilterComponent*>(graphEditor->getLassoSelection().getSelectedItem(0));
      
      if (selectedItem)
      {
        FaustAudioProcessor* faustProc = dynamic_cast<FaustAudioProcessor*>(audioEngine.getDoc().getNodeForId(selectedItem->filterID)->getProcessor());
        
        if (faustProc)
        {
          selectedFaustAudioProcessor = faustProc;
          browser->goToURL(faustProc->getFactory()->getSVGPath());
          return;
        }
      }
    }
  }
  
  selectedFaustAudioProcessor = nullptr;
  browser->goToURL("");
}