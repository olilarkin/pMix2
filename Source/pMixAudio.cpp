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