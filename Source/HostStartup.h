#ifndef __HOSTSTARTUP_JUCEHEADER__
#define __HOSTSTARTUP_JUCEHEADER__

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainHostWindow.h"
#include "InternalFilters.h"

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
  
  ApplicationCommandManager commandManager;
  ScopedPointer<ApplicationProperties> appProperties;
  LookAndFeel_V3 lookAndFeel;
  
private:
  ScopedPointer<MainHostWindow> mainWindow;
//  SplashScreen* splash;
};

#endif //__HOSTSTARTUP_JUCEHEADER__