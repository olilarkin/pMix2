/*
  ==============================================================================
 
    pMixApp.h
    Author:  Oliver Larkin
  
  ==============================================================================
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "pMixMainAppWindow.h"

class pMixApp : public JUCEApplication
{
public:
  pMixApp();
  void initialise (const String& commandLine);
  void shutdown();
  void systemRequestedQuit();
  const String getApplicationName();
  const String getApplicationVersion();
  bool moreThanOneInstanceAllowed();
  
  AudioDeviceManager deviceManager;
  ApplicationCommandManager commandManager;

private:
  ScopedPointer<MainAppWindow> mainWindow;
  AudioProcessorPlayer graphPlayer;
  PMixAudioEngine audioEngine;
//  SplashScreen* splash;
};
