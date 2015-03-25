/*
  ==============================================================================

    pMixAudioEngine.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixAudioEngine.h"
#include "pMixCommandIDs.h"

PMixAudioEngine::PMixAudioEngine()
: doc(*this)
, faustDSPFormat(doc.getLibraryPath())
{
  // initialise our settings file..
  
  PropertiesFile::Options options;
  options.applicationName     = "pMix";
  options.filenameSuffix      = "settings";
  options.osxLibrarySubFolder = "Application Support";
#ifdef JUCE_MAC
  options.folderName = "pMix2";
#endif
  appProperties = new ApplicationProperties();
  appProperties->setStorageParameters (options);
  
  formatManager.addDefaultFormats();
  formatManager.addFormat (new InternalPluginFormat());
  InternalPluginFormat internalFormat;
  internalFormat.getAllTypes (internalTypes);
  
  ScopedPointer<XmlElement> savedPluginList (getAppProperties().getUserSettings()->getXmlValue ("pluginList"));
  
  if (savedPluginList != nullptr)
    knownPluginList.recreateFromXml (*savedPluginList);
    
  ScopedPointer<PluginDirectoryScanner> scanner = new PluginDirectoryScanner(knownFaustDSPList, faustDSPFormat, faustDSPFormat.getDefaultLocationsToSearch(), true, File::nonexistent);
  
  String pluginBeingScanned;

  while (scanner->scanNextFile(true, pluginBeingScanned)) {
    LOG(pluginBeingScanned);
  }
  
  pluginSortMethod = (KnownPluginList::SortMethod) getAppProperties().getUserSettings()->getIntValue ("pluginSortMethod", KnownPluginList::sortByFormat);

#if JUCE_MAC
  knownPluginList.addToBlacklist("/Library/Audio/Plug-Ins/VST/pMixPlugin.vst");
#endif
  knownPluginList.addChangeListener (this);
}

PMixAudioEngine::~PMixAudioEngine()
{
  knownPluginList.removeChangeListener (this);  
  doc.clear();
  appProperties = nullptr;
}

void PMixAudioEngine::changeListenerCallback (ChangeBroadcaster* broadcaster)
{
  if (broadcaster == &knownPluginList)
  {
    // save the plugin list every time it gets chnaged, so that if we're scanning
    // and it crashes, we've still saved the previous ones
    ScopedPointer<XmlElement> savedPluginList (knownPluginList.createXml());
    
    if (savedPluginList != nullptr)
    {
      getAppProperties().getUserSettings()->setValue ("pluginList", savedPluginList);
      getAppProperties().saveIfNeeded();
    }
  }
}

void PMixAudioEngine::setPluginSortMethod(KnownPluginList::SortMethod sortMethod)
{
  pluginSortMethod = sortMethod;
  getAppProperties().getUserSettings()->setValue ("pluginSortMethod", (int) pluginSortMethod);
}

void PMixAudioEngine::createDeviceMenu (PopupMenu& m) const
{
  PopupMenu ioMenu;
  
  ioMenu.addItem (CommandIDs::newAudioInput, internalTypes.getUnchecked(0)->name);
  ioMenu.addItem (CommandIDs::newAudioOutput, internalTypes.getUnchecked(1)->name);
  ioMenu.addItem (CommandIDs::newMIDIInput, internalTypes.getUnchecked(2)->name);
  ioMenu.addItem (CommandIDs::newMIDIOutput, internalTypes.getUnchecked(3)->name);

  m.addSubMenu("I/O", ioMenu);
  
  PopupMenu faustMenu;
  faustMenu.addItem(CommandIDs::newFaustEffect, "Effect");
  faustMenu.addItem(CommandIDs::newFaustEffect, "Synth");
  faustMenu.addSeparator();
  
  knownFaustDSPList.addToMenu(faustMenu, KnownPluginList::sortAlphabetically);
  
  m.addSubMenu("Faust", faustMenu);
  
  PopupMenu pluginsMenu;
  knownPluginList.addToMenu (pluginsMenu, pluginSortMethod);

  m.addSubMenu("Plugins", pluginsMenu);
}

const PluginDescription* PMixAudioEngine::getChosenType (const int menuID) const
{
  switch (menuID)
  {
    case CommandIDs::newAudioInput: return internalTypes[0];
    case CommandIDs::newAudioOutput: return internalTypes[1];
    case CommandIDs::newMIDIInput: return internalTypes[2];
    case CommandIDs::newMIDIOutput: return internalTypes[3];
    case CommandIDs::newFaustEffect: return internalTypes[4];
    default:
    {
      int result = knownFaustDSPList.getIndexChosenByMenu(menuID);
      
      if (result > -1)
        return knownFaustDSPList.getType(result);
    
      return knownPluginList.getType (knownPluginList.getIndexChosenByMenu (menuID));
    }
  }
}

