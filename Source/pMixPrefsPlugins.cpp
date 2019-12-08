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

#include "pMixPrefsPlugins.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
PMixPrefsPlugins::PMixPrefsPlugins (PMixAudioEngine& audioEngine)
    : audioEngine(audioEngine)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    pluginListComponent = std::make_unique<PluginListComponent>(audioEngine.getFormatManager(), audioEngine.getKnownPluginList(), audioEngine.getDMPFile(), audioEngine.getAppProperties().getUserSettings());
    addAndMakeVisible (*pluginListComponent);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

PMixPrefsPlugins::~PMixPrefsPlugins()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    pluginListComponent = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void PMixPrefsPlugins::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void PMixPrefsPlugins::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    pluginListComponent->setBounds (8, 8, getWidth() - 16, getHeight() - 16);
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

<JUCER_COMPONENT documentType="Component" className="PMixPrefsPlugins" componentName=""
                 parentClasses="public Component" constructorParams="PMixAudioEngine&amp; audioEngine"
                 variableInitialisers="audioEngine(audioEngine)" snapPixels="8"
                 snapActive="1" snapShown="1" overlayOpacity="0.330" fixedSize="0"
                 initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ffffffff"/>
  <GENERICCOMPONENT name="" id="e9532f1fb8d54751" memberName="pluginListComponent"
                    virtualName="" explicitFocusOrder="0" pos="8 8 16M 16M" class="PluginListComponent"
                    params="audioEngine.getFormatManager(), audioEngine.getKnownPluginList(), audioEngine.getDMPFile(), audioEngine.getAppProperties().getUserSettings()"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
