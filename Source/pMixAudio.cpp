/*
  ==============================================================================

    pMixAudio.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixAudio.h"

PMixAudio::PMixAudio()
: doc(*this)
{
  formatManager.addDefaultFormats();
  formatManager.addFormat (new InternalPluginFormat());
  
  InternalPluginFormat internalFormat;
  internalFormat.getAllTypes (internalTypes);
  
  ScopedPointer<XmlElement> savedPluginList (getAppProperties().getUserSettings()->getXmlValue ("pluginList"));
  
  if (savedPluginList != nullptr)
    knownPluginList.recreateFromXml (*savedPluginList);
  
  pluginSortMethod = (KnownPluginList::SortMethod) getAppProperties().getUserSettings()
  ->getIntValue ("pluginSortMethod", KnownPluginList::sortByManufacturer);
  
  knownPluginList.addChangeListener (this);
}

PMixAudio::~PMixAudio()
{
  knownPluginList.removeChangeListener (this);
}

void PMixAudio::changeListenerCallback (ChangeBroadcaster* broadcaster)
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

void PMixAudio::setPluginSortMethod(KnownPluginList::SortMethod sortMethod)
{
  pluginSortMethod = sortMethod;
  getAppProperties().getUserSettings()->setValue ("pluginSortMethod", (int) pluginSortMethod);
}

void PMixAudio::addPluginsToMenu (PopupMenu& m) const
{
  for (int i = 0; i < internalTypes.size(); ++i)
    m.addItem (i + 1, internalTypes.getUnchecked(i)->name);

  m.addSeparator();

  knownPluginList.addToMenu (m, pluginSortMethod);
}

const PluginDescription* PMixAudio::getChosenType (const int menuID) const
{
  if (menuID >= 1 && menuID < 1 + internalTypes.size())
    return internalTypes [menuID - 1];

  return knownPluginList.getType (knownPluginList.getIndexChosenByMenu (menuID));
}
