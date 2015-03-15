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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "pMixRHPTabContainer.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
PMixTabContainer::PMixTabContainer (PMixAudioEngine& audioEngine, GraphEditor& graphEditor)
{
    addAndMakeVisible (tabbedComponent = new TabbedComponent (TabbedButtonBar::TabsAtTop));
    tabbedComponent->setTabBarDepth (23);
    tabbedComponent->addTab (TRANS("Code Editor"), Colours::lightgrey, new CodeEditor (audioEngine, graphEditor), true);
    tabbedComponent->addTab (TRANS("Interpolation Space"), Colours::lightgrey, new InterpolationSpace (audioEngine, graphEditor), true);
    tabbedComponent->setCurrentTabIndex (0);


    //[UserPreSize]
    tabbedComponent->setOutline(0.);
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

PMixTabContainer::~PMixTabContainer()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    tabbedComponent = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void PMixTabContainer::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void PMixTabContainer::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    tabbedComponent->setBounds (0, 0, getWidth() - 0, getHeight() - 0);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PMixTabContainer" componentName=""
                 parentClasses="public Component" constructorParams="PMixAudioEngine&amp; audioEngine, GraphEditor&amp; graphEditor"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ffffffff"/>
  <TABBEDCOMPONENT name="new tabbed component" id="62eb79f9020fc44b" memberName="tabbedComponent"
                   virtualName="" explicitFocusOrder="0" pos="0 0 0M 0M" orientation="top"
                   tabBarDepth="23" initialTab="0">
    <TAB name="Code Editor" colour="ffd3d3d3" useJucerComp="0" contentClassName="CodeEditor"
         constructorParams="audioEngine, graphEditor" jucerComponentFile=""/>
    <TAB name="Interpolation Space" colour="ffd3d3d3" useJucerComp="0"
         contentClassName="InterpolationSpace" constructorParams="audioEngine, graphEditor"
         jucerComponentFile=""/>
  </TABBEDCOMPONENT>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
