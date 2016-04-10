#ifndef __INTERNALFILTERS_JUCEHEADER__
#define __INTERNALFILTERS_JUCEHEADER__

#include "JuceHeader.h"

class InternalPluginFormat   : public AudioPluginFormat
{
public:
  InternalPluginFormat();
  ~InternalPluginFormat() {}

  enum InternalFilterType
  {
    audioInputFilter = 0,
    audioOutputFilter,
    midiInputFilter,
    midiOutputFilter,
    faustEffect,

    endOfFilterTypes
  };

  const PluginDescription* getDescriptionFor (const InternalFilterType type);

  void getAllTypes (OwnedArray <PluginDescription>& results);

  String getName() const override                                      { return "Internal"; }
  bool fileMightContainThisPluginType (const String&) override         { return true; }
  FileSearchPath getDefaultLocationsToSearch() override                { return FileSearchPath(); }
  bool canScanForPlugins() const override                              { return false; }
  void findAllTypesForFile (OwnedArray <PluginDescription>&, const String&) override     {}
  bool doesPluginStillExist (const PluginDescription&) override        { return true; }
  String getNameOfPluginFromIdentifier (const String& fileOrIdentifier) override   { return fileOrIdentifier; }
  bool pluginNeedsRescanning (const PluginDescription&) override       { return false; }
  StringArray searchPathsForPlugins (const FileSearchPath& directoriesToSearch, const bool recursive, bool allowPluginsWhichRequireAsynchronousInstantiation = false) override     { return StringArray(); } 
  
  static bool isInternalFormat(String name);
  
private:
  void createPluginInstance (const PluginDescription&, double initialSampleRate, int initialBufferSize, void* userData, void (*callback) (void*, AudioPluginInstance*, const String&)) override;
  
  bool requiresUnblockedMessageThreadDuringCreation (const PluginDescription&) const noexcept override { return false; }
  
  PluginDescription audioInDesc;
  PluginDescription audioOutDesc;
  PluginDescription midiInDesc;
  PluginDescription midiOutDesc;
  PluginDescription faustDesc;
};


#endif   // __INTERNALFILTERS_JUCEHEADER__
