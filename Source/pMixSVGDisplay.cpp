/*
  ==============================================================================

    pMixSVGDisplay.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixSVGDisplay.h"
#include "pMixGraphEditor.h"

SVGDisplay::SVGDisplay(PMixAudioEngine& audioEngine, GraphEditor& graphEditor)
: audioEngine(audioEngine)
, graphEditor(graphEditor)
, svgDrawable(nullptr)
, browser(nullptr)
{
  //browser = new WebBrowserComponent();
  
  if (browser) {
    addAndMakeVisible (browser);
  }
  
  graphEditor.addChangeListener(this);
  audioEngine.getDoc().addChangeListener(this);
  
  clearDisplay();
}

SVGDisplay::~SVGDisplay()
{
  graphEditor.removeChangeListener(this);
  audioEngine.getDoc().removeChangeListener(this);
  
  if (svgDrawable) {
    svgDrawable = nullptr;
  }
}

void SVGDisplay::paint (Graphics& g)
{
  if (browser == nullptr)
  {
    g.fillAll (Colours::white);
    
    if (svgDrawable)
      svgDrawable->drawWithin(g, Rectangle<float>(0, 0, getWidth(), getHeight()), RectanglePlacement::Flags::centred, 1.0f);
  }
}

void SVGDisplay::resized()
{
  Rectangle<int> r (getLocalBounds());
  
  if (browser)
    browser->setBounds (r);
}

void SVGDisplay::changeListenerCallback (ChangeBroadcaster* source)
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
      NodeComponent* selectedItem = dynamic_cast<NodeComponent*>(pgraphEditor->getLassoSelection().getSelectedItem(0));
      
      if (selectedItem)
      {
        FaustAudioPluginInstance* faustProc = dynamic_cast<FaustAudioPluginInstance*>(audioEngine.getDoc().getNodeForId(selectedItem->nodeID)->getProcessor());
        
        if (faustProc)
        {
          if (!faustProc->getHighlight())
          {
            if (browser)
              browser->goToURL(audioEngine.getDoc().getLibraryPath() + "wait.html");
            else
            {
              //TODO:SVG wait
            }
          }
          else
            clearDisplay();
          
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
      NodeComponent* selectedItem = dynamic_cast<NodeComponent*>(graphEditor.getLassoSelection().getSelectedItem(0));

      if (selectedItem) {
        if (browser)
          loadSVG(thread->getFactory()->getHTMLURI());
        else
          loadSVG(thread->getFactory()->getSVGFile().getFullPathName());

        return;
      }
    }
  }

}

void SVGDisplay::clearDisplay()
{
  if (browser) {
    browser->goToURL("");
  }
  else {
    svgDrawable = nullptr;
    repaint();
  }
}

void SVGDisplay::loadSVG(const String& url)
{
  if (browser)
    browser->goToURL(url);
  else
  {
    svgDrawable = Drawable::createFromImageFile(File(url));
    repaint();
  }
}
