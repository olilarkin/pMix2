#include "pMixApp.h"

#if ! (JUCE_PLUGINHOST_VST || JUCE_PLUGINHOST_VST3 || JUCE_PLUGINHOST_AU)
#error "If you're building the audio plugin host, you probably want to enable VST and/or AU support"
#endif

pMixApp::pMixApp() {}

void pMixApp::initialise (const String& commandLine)
{
//  splash = new SplashScreen ("pMix",
//                             ImageCache::getFromMemory (BinaryData::pMix_about_png, BinaryData::pMix_about_pngSize),
//                             true);
//  
//  splash->deleteAfterDelay (RelativeTime::seconds (4), false);
  


  LookAndFeel::setDefaultLookAndFeel (&lookAndFeel);

  mainWindow = new MainAppWindow(&deviceManager);

  commandManager.registerAllCommandsForTarget (this);

//  mainWindow->menuItemsChanged();

//  if (commandLine.isNotEmpty() && ! commandLine.trimStart().startsWith ("-") && mainWindow->getMainComponent() != nullptr)
//  {
//    mainWindow->getMainComponent()->getDoc().loadFrom (File::getCurrentWorkingDirectory().getChildFile (commandLine), true);
//  }
  
  ScopedPointer<XmlElement> savedAudioState (getAppProperties().getUserSettings()->getXmlValue ("audioDeviceState"));
  
  mainWindow->getAudio().getDoc().initialize();
  deviceManager.initialise (256, 256, savedAudioState, true);
}

void pMixApp::shutdown()
{
  mainWindow = nullptr;
  LookAndFeel::setDefaultLookAndFeel (nullptr);
}

void pMixApp::systemRequestedQuit()
{
  if (mainWindow != nullptr)
    mainWindow->tryToQuitApplication();
  else
    JUCEApplicationBase::quit();
}

//void pMixApp::showAudioSettings()
//{
//  AudioDeviceSelectorComponent audioSettingsComp (deviceManager,
//                                                  0, 256,
//                                                  0, 256,
//                                                  true, true, true, false);
//  
//  audioSettingsComp.setSize (500, 450);
//  
//  DialogWindow::LaunchOptions o;
//  o.content.setNonOwned (&audioSettingsComp);
//  o.dialogTitle                   = "Audio Settings";
//  o.componentToCentreAround       = mainWindow->getMainComponent();
//  o.dialogBackgroundColour        = Colours::grey;
//  o.escapeKeyTriggersCloseButton  = true;
//  o.useNativeTitleBar             = true;
//  o.resizable                     = false;
//  
//  o.runModal();
//  
//  ScopedPointer<XmlElement> audioState (deviceManager.createStateXml());
//  
//  getAppProperties().getUserSettings()->setValue ("audioDeviceState", audioState);
//  getAppProperties().getUserSettings()->saveIfNeeded();
//  
//  MainComponent* const mainComponent = mainWindow->getMainComponent();
//  
//  if (mainComponent != nullptr)
//    mainComponent->getDoc().removeIllegalConnections();
//}

const String pMixApp::getApplicationName()       { return "pMix"; }
const String pMixApp::getApplicationVersion()    { return ProjectInfo::versionString; }
bool pMixApp::moreThanOneInstanceAllowed()       { return false; }

static pMixApp& getApp()                         { return *dynamic_cast<pMixApp*>(JUCEApplication::getInstance()); }
ApplicationCommandManager& getCommandManager()   { return getApp().commandManager; }
ApplicationProperties& getAppProperties()        { return *getApp().appProperties; }


// This kicks the whole thing off..
START_JUCE_APPLICATION (pMixApp)
