/*
  ==============================================================================

    pMixWebBrowser.cpp
    Created: 1 Feb 2015 9:43:23pm
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixWebBrowser.h"
#include "pMixGraphEditor.h"

WebBrowser::WebBrowser(PMixAudioEngine& audioEngine, GraphEditor& graphEditor)
: audioEngine(audioEngine)
, graphEditor(graphEditor)
, selectedFaustAudioPluginInstance(nullptr)
{
  addAndMakeVisible (browser = new WebBrowserComponent ());
  graphEditor.addChangeListener(this);
  browser->goToURL("");
}

WebBrowser::~WebBrowser()
{
  graphEditor.removeChangeListener(this);
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
        FaustAudioPluginInstance* faustProc = dynamic_cast<FaustAudioPluginInstance*>(audioEngine.getDoc().getNodeForId(selectedItem->nodeId)->getProcessor());
        
        if (faustProc)
        {
          selectedFaustAudioPluginInstance = faustProc;
          browser->goToURL(faustProc->getFactory()->getSVGFileURI());
          return;
        }
      }
    }
  }
  
  selectedFaustAudioPluginInstance = nullptr;
  browser->goToURL("");
}