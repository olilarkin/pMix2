#include "HostStartup.h"

#if ! (JUCE_PLUGINHOST_VST || JUCE_PLUGINHOST_VST3 || JUCE_PLUGINHOST_AU)
#error "If you're building the audio plugin host, you probably want to enable VST and/or AU support"
#endif

pMixApp::pMixApp() {}

void pMixApp::initialise (const String& commandLine)
{
  splash = new SplashScreen ("pMix",
                             ImageCache::getFromMemory (BinaryData::pMix_about_png, BinaryData::pMix_about_pngSize),
                             true);
  
  splash->deleteAfterDelay (RelativeTime::seconds (4), false);
  
  // initialise our settings file..

  PropertiesFile::Options options;
  options.applicationName     = "pMix";
  options.filenameSuffix      = "settings";
  options.osxLibrarySubFolder = "Preferences";

  appProperties = new ApplicationProperties();
  appProperties->setStorageParameters (options);

  LookAndFeel::setDefaultLookAndFeel (&lookAndFeel);

  mainWindow = new MainHostWindow();
  mainWindow->setUsingNativeTitleBar (true);

  commandManager.registerAllCommandsForTarget (this);
  //commandManager.registerAllCommandsForTarget (mainWindow);

  mainWindow->menuItemsChanged();

  if (commandLine.isNotEmpty() && ! commandLine.trimStart().startsWith ("-") && mainWindow->getGraphEditor() != nullptr)
  {
    mainWindow->getGraphEditor()->graph.loadFrom (File::getCurrentWorkingDirectory().getChildFile (commandLine), true);
  }
}

void pMixApp::shutdown()
{
  mainWindow = nullptr;
  appProperties = nullptr;
  LookAndFeel::setDefaultLookAndFeel (nullptr);
}

void pMixApp::systemRequestedQuit()
{
  if (mainWindow != nullptr)
    mainWindow->tryToQuitApplication();
  else
    JUCEApplicationBase::quit();
}

const String pMixApp::getApplicationName()       { return "pMix"; }
const String pMixApp::getApplicationVersion()    { return ProjectInfo::versionString; }
bool pMixApp::moreThanOneInstanceAllowed()       { return false; }

static pMixApp& getApp()                      { return *dynamic_cast<pMixApp*>(JUCEApplication::getInstance()); }
ApplicationCommandManager& getCommandManager()      { return getApp().commandManager; }
ApplicationProperties& getAppProperties()           { return *getApp().appProperties; }


// This kicks the whole thing off..
START_JUCE_APPLICATION (pMixApp)
