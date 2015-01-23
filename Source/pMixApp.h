#ifndef __PMIXAPP_JUCEHEADER__
#define __PMIXAPP_JUCEHEADER__

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainHostWindow.h"
#include "InternalFilters.h"
#include "LookAndFeel.h"
#include "pMixAppProperties.h"

class pMixApp : public JUCEApplication
              , public pMixAppProperties
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
  pMixLookAndFeel lookAndFeel;
  
private:
  ScopedPointer<MainHostWindow> mainWindow;
  AudioDeviceManager deviceManager;
//  SplashScreen* splash;
};

#endif //__PMIXAPP_JUCEHEADER__