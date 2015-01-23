#ifndef __PMIXAPP_JUCEHEADER__
#define __PMIXAPP_JUCEHEADER__

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainHostWindow.h"
#include "InternalFilters.h"
#include "LookAndFeel.h"

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
  void showAudioSettings();

  ApplicationCommandManager commandManager;
  ScopedPointer<ApplicationProperties> appProperties;
  pMixLookAndFeel lookAndFeel;
  
private:
  ScopedPointer<MainHostWindow> mainWindow;
  AudioDeviceManager deviceManager;
//  SplashScreen* splash;
};

#endif //__PMIXAPP_JUCEHEADER__