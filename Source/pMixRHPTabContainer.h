/*
  ==============================================================================
 
    pMixRHPTabContainer.h
    Author:  Oliver Larkin
 
  ==============================================================================
 */

#ifndef PMIXRHPTAB_H_INCLUDED
#define PMIXRHPTAB_H_INCLUDED

#include "JuceHeader.h"
#include "pMixInterpolationSpace.h"
#include "pMixCodeEditor.h"
#include "pMixGraphEditor.h"

class PMixTabContainer  : public Component
{
public:
  PMixTabContainer (PMixAudioEngine& audioEngine, GraphEditor& graphEditor, CodeEditor& codeEditor, InterpolationSpace& iSpace);
  ~PMixTabContainer();

  CodeEditor* getCodeEditor() { return dynamic_cast<CodeEditor*>(tabbedComponent->getTabContentComponent(0)); }

  void paint (Graphics& g);
  void resized();
  
  void addTabForContent(Component& content, int window);
  void removeTabForContent(Component& content);

private:
  ScopedPointer<TabbedComponent> tabbedComponent;
  InterpolationSpace& iSpace;
  CodeEditor& codeEditor;
  WebBrowserComponent docsBrowser;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PMixTabContainer)
};

#endif //PMIXRHPTAB_H_INCLUDED
