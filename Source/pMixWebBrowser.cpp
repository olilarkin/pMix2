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
{
  addAndMakeVisible (browser = new WebBrowserComponent ());
  graphEditor.addChangeListener(this);
  audioEngine.getDoc().addChangeListener(this);
  browser->goToURL("");
}

WebBrowser::~WebBrowser()
{
  graphEditor.removeChangeListener(this);
  audioEngine.getDoc().removeChangeListener(this);
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
  PMixDocument* doc = dynamic_cast<PMixDocument*>(source);

  // every time the doc changes, loop over nodes and register as a listener to all the faust nodes.
  if (doc) {
    for (int i = 0; i <audioEngine.getGraph().getNumNodes(); i++) {
      FaustAudioPluginInstance* faustProc = dynamic_cast<FaustAudioPluginInstance*>(audioEngine.getDoc().getNode(i)->getProcessor());

      if (faustProc)
        faustProc->getFactory()->registerSVGThreadListenser(this);
    }
    
    return;
  }
  
  GraphEditor* pgraphEditor = dynamic_cast<GraphEditor*>(source);
  
  if (pgraphEditor)
  {
    if(pgraphEditor->getLassoSelection().getNumSelected() == 1)
    {
      FilterComponent* selectedItem = dynamic_cast<FilterComponent*>(pgraphEditor->getLassoSelection().getSelectedItem(0));
      
      if (selectedItem)
      {
        FaustAudioPluginInstance* faustProc = dynamic_cast<FaustAudioPluginInstance*>(audioEngine.getDoc().getNodeForId(selectedItem->nodeId)->getProcessor());
        
        if (faustProc)
        {
          if (!faustProc->getHighlight())
          {
            browser->goToURL(audioEngine.getDoc().getLibraryPath() + "wait.html");
          }
          else
            browser->goToURL("");
          
          return;
        }
      }
    }
  }
  
  FaustgenFactory::SVGRenderThread* thread = dynamic_cast<FaustgenFactory::SVGRenderThread*>(source);
  
  // if an SVGRenderThread has triggered this change message
  if (thread)
  {
    // if a single item is selected in the graph editor update the browser
    if(graphEditor.getLassoSelection().getNumSelected() == 1)
    {
      FilterComponent* selectedItem = dynamic_cast<FilterComponent*>(graphEditor.getLassoSelection().getSelectedItem(0));

      if (selectedItem) {
        browser->goToURL(thread->getFactory()->getHTMLURI());
        return;
      }
    }
  }

  browser->goToURL("");
}