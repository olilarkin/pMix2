/*
 ==============================================================================
 
 pMixApp.h
 Author:  Oliver Larkin
 
 The top level class for the app.
 
 ==============================================================================
 */

#ifndef PMIXAPP_H_INCLUDED
#define PMIXAPP_H_INCLUDED

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

#endif //PMIXAPP_H_INCLUDED