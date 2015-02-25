/*
  ==============================================================================

    FaustPluginFormat.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#ifndef FAUSTPLUGINFORMAT_H_INCLUDED
#define FAUSTPLUGINFORMAT_H_INCLUDED

#include "JuceHeader.h"
#include "FaustAudioProcessor.h"

class FaustPluginFormat   : public AudioPluginFormat
{
private:
  String faustLibraryPath;
  
public:
  FaustPluginFormat(String faustLibraryPath)
  : faustLibraryPath(faustLibraryPath)
  {};
  
  ~FaustPluginFormat() {}

  void getAllTypes (OwnedArray <PluginDescription>& results);

  String getName() const override
  {
    return "FAUST";
  }
  
  bool fileMightContainThisPluginType (const String& fileOrIdentifier) override
  {
    const File f (File::createFileWithoutCheckingPath (fileOrIdentifier));
    return f.existsAsFile() && f.hasFileExtension (".dsp");
  }
  
  FileSearchPath getDefaultLocationsToSearch() override
  {
    return FileSearchPath("/Users/oli/Dev/MyOWL/OwlWare/Libraries/OwlPatches/OliLarkin");
  }
  
  bool canScanForPlugins() const override
  {
    return true;
  }
  
  void findAllTypesForFile (OwnedArray <PluginDescription>& results, const String& fileOrIdentifier) override
  {
    if (! fileMightContainThisPluginType (fileOrIdentifier))
      return;
    
    PluginDescription desc;
    desc.fileOrIdentifier = fileOrIdentifier;
    desc.uid = 0;
    
    ScopedPointer<FaustAudioProcessor> instance (dynamic_cast<FaustAudioProcessor*> (createInstanceFromDescription (desc, 44100., 512)));
    
    if (instance == nullptr)
      return;
    
    if (instance->getHighlight())
      return;
    
    instance->fillInPluginDescription (desc);
    
    results.add (new PluginDescription (desc));
  }
  
  bool doesPluginStillExist (const PluginDescription& desc) override
  {
    return File::createFileWithoutCheckingPath (desc.fileOrIdentifier).exists();
  }
  
  String getNameOfPluginFromIdentifier (const String& fileOrIdentifier) override
  {
    return fileOrIdentifier;
  }
  
  bool pluginNeedsRescanning (const PluginDescription& desc) override
  {
    return File (desc.fileOrIdentifier).getLastModificationTime() != desc.lastFileModTime;
  }
  
  StringArray searchPathsForPlugins (const FileSearchPath& directoriesToSearch, const bool recursive) override
  {
    StringArray results;

    for (int j = 0; j < directoriesToSearch.getNumPaths(); ++j)
      recursiveFileSearch (results, directoriesToSearch[j], recursive);

    return results;
  }
  
  void recursiveFileSearch (StringArray& results, const File& dir, const bool recursive)
  {
    DirectoryIterator iter (dir, false, "*", File::findFilesAndDirectories);
    
    while (iter.next())
    {
      const File f (iter.getFile());
      bool isPlugin = false;
      
      if (fileMightContainThisPluginType (f.getFullPathName()))
      {
        isPlugin = true;
        results.add (f.getFullPathName());
      }
      
      if (recursive && (! isPlugin) && f.isDirectory())
        recursiveFileSearch (results, f, true);
    }
  }
  
  AudioPluginInstance* createInstanceFromDescription (const PluginDescription& desc, double initialSampleRate, int initialBufferSize) override
  {
    ScopedPointer<FaustAudioProcessor> result;
    
    if (fileMightContainThisPluginType (desc.fileOrIdentifier))
    {
      File dspFile = File(desc.fileOrIdentifier);
      
      result = new FaustAudioProcessor();
      result->initialize(faustLibraryPath);
      result->getFactory()->addLibraryPath(dspFile.getParentDirectory());
      result->getFactory()->updateSourceCode(dspFile.loadFileAsString(), result);
      result->prepareToPlay(initialSampleRate, initialBufferSize);
    }
    
    return result.release();
  };
};



#endif  // FAUSTPLUGINFORMAT_H_INCLUDED
