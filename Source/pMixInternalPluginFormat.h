/*
  ==============================================================================

    pMixInternalPluginFormat.h
    Author:  Oliver Larkin
 
  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class InternalPluginFormat   : public AudioPluginFormat
{
public:
  InternalPluginFormat();
  ~InternalPluginFormat() {}

  enum InternalNodeType
  {
    audioInputNode = 0,
    audioOutputNode,
    midiInputNode,
    midiOutputNode,
    endOfNodeTypes
  };

  const PluginDescription* getDescriptionFor (const InternalNodeType type);

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
  bool isTrivialToScan() const override { return true; }

private:
  std::unique_ptr<AudioPluginInstance> createInstance (const String& name);
  void createPluginInstance (const PluginDescription&, double initialSampleRate, int initialBufferSize, PluginCreationCallback) override;

  bool requiresUnblockedMessageThreadDuringCreation (const PluginDescription&) const noexcept override { return false; }
  
  PluginDescription audioInDesc;
  PluginDescription audioOutDesc;
  PluginDescription midiInDesc;
  PluginDescription midiOutDesc;
};


