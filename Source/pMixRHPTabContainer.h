/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.1.1

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-13 by Raw Material Software Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_8424B547AA1C0712__
#define __JUCE_HEADER_8424B547AA1C0712__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "pMixInterpolationSpace.h"
#include "pMixCodeEditor.h"
#include "pMixGraphEditor.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class PMixTabContainer  : public Component
{
public:
    //==============================================================================
    PMixTabContainer (PMixAudioEngine& audioEngine, GraphEditor& graphEditor);
    ~PMixTabContainer();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    CodeEditor* getCodeEditor() { return dynamic_cast<CodeEditor*>(tabbedComponent->getTabContentComponent(0)); }
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<TabbedComponent> tabbedComponent;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PMixTabContainer)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_8424B547AA1C0712__
