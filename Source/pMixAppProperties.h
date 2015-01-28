/*
  ==============================================================================

    pMixAppProperties.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#ifndef PMIXAPPPROPERTIES_H_INCLUDED
#define PMIXAPPPROPERTIES_H_INCLUDED

//#include "../JuceLibraryCode/JuceHeader.h"

class pMixAppPropertiesHolder
{
public:
  pMixAppPropertiesHolder()
  {
    // initialise our settings file..
    
    PropertiesFile::Options options;
    options.applicationName     = "pMix";
    options.filenameSuffix      = "settings";
    options.osxLibrarySubFolder = "Preferences";
    
    appProperties = new ApplicationProperties();
    appProperties->setStorageParameters (options);
  }
  
  ~pMixAppPropertiesHolder()
  {
    appProperties = nullptr;
  }
  
  ScopedPointer<ApplicationProperties> appProperties;
};

#endif  // PMIXAPPPROPERTIES_H_INCLUDED
