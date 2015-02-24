/*
  ==============================================================================

    pMixAudioEngine.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixAudioEngine.h"
#include "pMixCommandIDs.h"

FaustDSPFileFilter PMixAudioEngine::DSPFileFilter;

PMixAudioEngine::PMixAudioEngine()
: thread("DSP folder watcher")
, faustDSPfiles(&DSPFileFilter, thread)
, doc(*this)
{
  // initialise our settings file..
  
  PropertiesFile::Options options;
  options.applicationName     = "pMix";
  options.filenameSuffix      = "settings";
  options.osxLibrarySubFolder = "Preferences";
  
  appProperties = new ApplicationProperties();
  appProperties->setStorageParameters (options);
  
  formatManager.addDefaultFormats();
  formatManager.addFormat (new InternalPluginFormat());
  
  InternalPluginFormat internalFormat;
  internalFormat.getAllTypes (internalTypes);
  
  ScopedPointer<XmlElement> savedPluginList (getAppProperties().getUserSettings()->getXmlValue ("pluginList"));
  
  if (savedPluginList != nullptr)
    knownPluginList.recreateFromXml (*savedPluginList);
  
  pluginSortMethod = (KnownPluginList::SortMethod) getAppProperties().getUserSettings()->getIntValue ("pluginSortMethod", KnownPluginList::sortByManufacturer);
  
  
  faustDSPfiles.setDirectory (File("/Users/oli/Dev/MyOWL/OwlWare/Libraries/OwlPatches/OliLarkin"), true, true);
  thread.startThread (3);
  
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
//    menuItemsChanged();
//    
//    // save the plugin list every time it gets chnaged, so that if we're scanning
//    // and it crashes, we've still saved the previous ones
//    ScopedPointer<XmlElement> savedPluginList (knownPluginList.createXml());
//    
//    if (savedPluginList != nullptr)
//    {
//      getAppProperties().getUserSettings()->setValue ("pluginList", savedPluginList);
//      getAppProperties().saveIfNeeded();
//    }
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
  
  int index = CommandIDs::faustDSPFilesMenu;

  for (int i=0; i<faustDSPfiles.getNumFiles(); i++)
    faustMenu.addItem(index++, faustDSPfiles.getFile(i).getFileNameWithoutExtension());

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
      if (menuID >= CommandIDs::faustDSPFilesMenu && menuID < CommandIDs::faustDSPFilesMenu + faustDSPfiles.getNumFiles()) return internalTypes[4];
      else
        return knownPluginList.getType (knownPluginList.getIndexChosenByMenu (menuID));
    }
  }
}