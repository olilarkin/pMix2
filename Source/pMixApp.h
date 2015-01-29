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
#include "pMixLookAndFeel.h"
#include "pMixAppProperties.h"
#include "pMixCommandManager.h"

class pMixApp : public JUCEApplication
              , public pMixAppPropertiesHolder
              , public pMixCommandManagerHolder
{
public:
  pMixApp();
  void initialise (const String& commandLine);
  void shutdown();
  void systemRequestedQuit();
  const String getApplicationName();
  const String getApplicationVersion();
  bool moreThanOneInstanceAllowed();
//  void showAudioSettings();

  pMixLookAndFeel lookAndFeel;
  
private:
  ScopedPointer<MainAppWindow> mainWindow;
  AudioDeviceManager deviceManager;
//  SplashScreen* splash;
};

#endif //PMIXAPP_H_INCLUDED