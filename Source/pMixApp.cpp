#include "pMixApp.h"

pMixApp::pMixApp()
{
}

void pMixApp::initialise (const String& commandLine)
{
//  splash = new SplashScreen ("pMix",
//                             ImageCache::getFromMemory (BinaryData::pMix_about_png, BinaryData::pMix_about_pngSize),
//                             true);
//  
//  splash->deleteAfterDelay (RelativeTime::seconds (4), false);
  


  mainWindow = new MainAppWindow(audioEngine);
  
  deviceManager.addAudioCallback (&graphPlayer);
  deviceManager.addMidiInputCallback (String::empty, &graphPlayer.getMidiMessageCollector());

  commandManager.registerAllCommandsForTarget (this);

  mainWindow->menuItemsChanged();

//  if (commandLine.isNotEmpty() && ! commandLine.trimStart().startsWith ("-") && mainWindow->getMainComponent() != nullptr)
//  {
//    mainWindow->getMainComponent()->getDoc().loadFrom (File::getCurrentWorkingDirectory().getChildFile (commandLine), true);
//  }
  
  ScopedPointer<XmlElement> savedAudioState (audioEngine.getAppProperties().getUserSettings()->getXmlValue ("audioDeviceState"));
  graphPlayer.setProcessor(&audioEngine.getGraph());
  audioEngine.getDoc().initialize();
  deviceManager.initialise (256, 256, savedAudioState, true);
}

void pMixApp::shutdown()
{
  deviceManager.removeAudioCallback (&graphPlayer);
  deviceManager.removeMidiInputCallback (String::empty, &graphPlayer.getMidiMessageCollector());
  graphPlayer.setProcessor (nullptr);

  mainWindow = nullptr;
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

static pMixApp& getApp()                         { return *dynamic_cast<pMixApp*>(JUCEApplication::getInstance()); }
ApplicationCommandManager& getCommandManager()   { return getApp().commandManager; }
AudioDeviceManager& getDeviceManager()           { return getApp().deviceManager; }

// This kicks the whole thing off..
START_JUCE_APPLICATION (pMixApp)
