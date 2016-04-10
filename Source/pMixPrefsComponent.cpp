/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.2.0

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "pMixPrefsComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
AudioDeviceManager& getDeviceManager();
//[/MiscUserDefs]

//==============================================================================
PMixPrefsComponent::PMixPrefsComponent (PMixAudioEngine& audioEngine)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (tabbedComponent = new TabbedComponent (TabbedButtonBar::TabsAtTop));
    tabbedComponent->setTabBarDepth (23);
    tabbedComponent->addTab (TRANS("General"), Colours::lightgrey, new PMixPrefsGeneral(), true);
    tabbedComponent->addTab (TRANS("Controllers"), Colours::lightgrey, new PMixPrefsControllers(), true);
  #ifndef PMIX_PLUGIN
    tabbedComponent->addTab (TRANS("Audio"), Colours::lightgrey, new PMixPrefsAudio (getDeviceManager()), true);
  #endif
    tabbedComponent->addTab (TRANS("Plug-Ins"), Colours::lightgrey, new PMixPrefsPlugins (audioEngine), true);
    tabbedComponent->addTab (TRANS("Colours"), Colours::lightgrey, new PMixPrefsColours(), true);
    tabbedComponent->setCurrentTabIndex (0);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (800, 600);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

PMixPrefsComponent::~PMixPrefsComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    tabbedComponent = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void PMixPrefsComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void PMixPrefsComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    tabbedComponent->setBounds (8, 8, getWidth() - 16, getHeight() - 16);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PMixPrefsComponent" componentName=""
                 parentClasses="public Component" constructorParams="PMixAudioEngine&amp; audioEngine"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="800" initialHeight="600">
  <BACKGROUND backgroundColour="ffffffff"/>
  <TABBEDCOMPONENT name="PrefsTabs" id="851c63a85ce6d934" memberName="tabbedComponent"
                   virtualName="" explicitFocusOrder="0" pos="8 8 16M 16M" orientation="top"
                   tabBarDepth="23" initialTab="0">
    <TAB name="General" colour="ffd3d3d3" useJucerComp="0" contentClassName="PMixPrefsGeneral"
         constructorParams="" jucerComponentFile=""/>
    <TAB name="Controllers" colour="ffd3d3d3" useJucerComp="0" contentClassName="PMixPrefsControllers"
         constructorParams="" jucerComponentFile=""/>
    <TAB name="Audio" colour="ffd3d3d3" useJucerComp="0" contentClassName="PMixPrefsAudio"
         constructorParams="getDeviceManager()" jucerComponentFile=""/>
    <TAB name="Plug-Ins" colour="ffd3d3d3" useJucerComp="0" contentClassName="PMixPrefsPlugins"
         constructorParams="audioEngine" jucerComponentFile=""/>
    <TAB name="Colours" colour="ffd3d3d3" useJucerComp="0" contentClassName="PMixPrefsColours"
         constructorParams="" jucerComponentFile=""/>
  </TABBEDCOMPONENT>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
