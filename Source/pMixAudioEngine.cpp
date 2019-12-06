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
{
  Logger::setCurrentLogger(&logger);
  
  LOG("pMix v 0.01");

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
  
  InternalPluginFormat* internalFormat = new InternalPluginFormat();
  internalFormat->getAllTypes (internalTypes);
  formatManager.addFormat(internalFormat);
  
  FaustPluginFormat* faustPluginFormat = new FaustPluginFormat(doc.getLibraryPath(), File(FAUST_DRAW_PATH));
  faustPluginFormat->getAllTypes(internalTypes);
  JITformatManager.addFormat(faustPluginFormat);
  
  std::unique_ptr<XmlElement> savedPluginList (getAppProperties().getUserSettings()->getXmlValue ("pluginList"));
  
  if (savedPluginList != nullptr)
    knownPluginList.recreateFromXml (*savedPluginList);
  
  scanFaustDSPFiles();
  
  String pluginBeingScanned;
  
  pluginSortMethod = (KnownPluginList::SortMethod) getAppProperties().getUserSettings()->getIntValue ("pluginSortMethod", KnownPluginList::sortByFileSystemLocation);
  setPluginSortMethod(pluginSortMethod);

#if JUCE_MAC
  knownPluginList.addToBlacklist("/Library/Audio/Plug-Ins/VST/pMixPlugin.vst");
#endif
  knownPluginList.addChangeListener (this);
}

PMixAudioEngine::~PMixAudioEngine()
{
  knownPluginList.removeChangeListener (this);  
  doc.clear();
  graph.clear();
  appProperties = nullptr;
}

void PMixAudioEngine::changeListenerCallback (ChangeBroadcaster* broadcaster)
{
  if (broadcaster == &knownPluginList)
  {
    // save the plugin list every time it gets chnaged, so that if we're scanning
    // and it crashes, we've still saved the previous ones
    std::unique_ptr<XmlElement> savedPluginList (knownPluginList.createXml());
    
    if (savedPluginList != nullptr)
    {
      getAppProperties().getUserSettings()->setValue ("pluginList", savedPluginList.get());
      getAppProperties().saveIfNeeded();
    }
  }
}

void PMixAudioEngine::setPluginSortMethod(KnownPluginList::SortMethod sortMethod)
{
  pluginSortMethod = sortMethod;
  getAppProperties().getUserSettings()->setValue ("pluginSortMethod", (int) pluginSortMethod);
}

void PMixAudioEngine::createNodeMenu (PopupMenu& m) const
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
  
  for(int i = 0; i< faustDSPFiles.size(); i++)
  {
    faustMenu.addItem(CommandIDs::faustDSPFiles + i, faustDSPFiles[i]->name);
  }
  
  m.addSubMenu("Faust", faustMenu);
 
  PopupMenu pluginsMenu;
  knownPluginList.addToMenu (pluginsMenu, pluginSortMethod);
  m.addSubMenu("Plugins", pluginsMenu);
}

const PluginDescription* PMixAudioEngine::getChosenType (const int menuID) const
{
  if(menuID >= CommandIDs::faustDSPFiles && menuID < CommandIDs::faustDSPFiles + faustDSPFiles.size())
  {
    return faustDSPFiles[menuID - CommandIDs::faustDSPFiles];
  }
  
  switch (menuID)
  {
    case CommandIDs::newAudioInput: return internalTypes[0];
    case CommandIDs::newAudioOutput: return internalTypes[1];
    case CommandIDs::newMIDIInput: return internalTypes[2];
    case CommandIDs::newMIDIOutput: return internalTypes[3];
    case CommandIDs::newFaustEffect: return internalTypes[4];
    default:
    {
      return knownPluginList.getType (knownPluginList.getIndexChosenByMenu (menuID));
    }
  }
}

std::unique_ptr<AudioPluginInstance> PMixAudioEngine::createPluginInstance(const PluginDescription& desc, String& errorMessage)
{
  std::unique_ptr<AudioPluginInstance> result = nullptr;
  
  if(desc.pluginFormatName == "FAUST")
  {
    result = JITformatManager.createPluginInstance (desc, getGraph().getSampleRate(), getGraph().getBlockSize(), errorMessage);
  }
  else
    result = formatManager.createPluginInstance (desc, getGraph().getSampleRate(), getGraph().getBlockSize(), errorMessage);
  
  return result;
}

void PMixAudioEngine::scanFaustDSPFiles()
{
  LOG("Scanning Faust .dsp files...");
  
  faustDSPFiles.clear();
  
  DirectoryIterator iter (File (DEFAULT_FAUST_DSP_SEARCHPATH), true, "*.dsp");
  
  while (iter.next())
  {
    File theFileItFound(iter.getFile());
    
    LOG("Found " + theFileItFound.getFileName());
    
    PluginDescription* desc = new PluginDescription();
    FaustAudioPluginInstance::fillInitialInPluginDescription(*desc);
    desc->name = theFileItFound.getFileName();
    desc->fileOrIdentifier = theFileItFound.getFullPathName();
    faustDSPFiles.add(desc);
  }
}
