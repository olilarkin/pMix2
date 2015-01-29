#ifndef __PMIXAPP_JUCEHEADER__
#define __PMIXAPP_JUCEHEADER__

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
  void showAudioSettings();

  pMixLookAndFeel lookAndFeel;
  
private:
  ScopedPointer<MainAppWindow> mainWindow;
  AudioDeviceManager deviceManager;
//  SplashScreen* splash;
};

#endif //__PMIXAPP_JUCEHEADER__