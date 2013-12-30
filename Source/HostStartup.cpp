#include "../JuceLibraryCode/JuceHeader.h"
#include "MainHostWindow.h"
#include "InternalFilters.h"

#if ! (JUCE_PLUGINHOST_VST || JUCE_PLUGINHOST_VST3 || JUCE_PLUGINHOST_AU)
#error "If you're building the audio plugin host, you probably want to enable VST and/or AU support"
#endif

class pMixApp  : public JUCEApplication
{
public:
  pMixApp() {}

  void initialise (const String& commandLine)
  {
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

  void shutdown()
  {
    mainWindow = nullptr;
    appProperties = nullptr;
    LookAndFeel::setDefaultLookAndFeel (nullptr);
  }

  void systemRequestedQuit()
  {
    if (mainWindow != nullptr)
      mainWindow->tryToQuitApplication();
    else
      JUCEApplicationBase::quit();
  }

  const String getApplicationName()       { return "pMix"; }
  const String getApplicationVersion()    { return ProjectInfo::versionString; }
  bool moreThanOneInstanceAllowed()       { return true; }

  ApplicationCommandManager commandManager;
  ScopedPointer<ApplicationProperties> appProperties;
  LookAndFeel_V3 lookAndFeel;

private:
  ScopedPointer<MainHostWindow> mainWindow;
};

static pMixApp& getApp()                      { return *dynamic_cast<pMixApp*>(JUCEApplication::getInstance()); }
ApplicationCommandManager& getCommandManager()      { return getApp().commandManager; }
ApplicationProperties& getAppProperties()           { return *getApp().appProperties; }


// This kicks the whole thing off..
START_JUCE_APPLICATION (pMixApp)
