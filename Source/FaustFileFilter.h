/*
  ==============================================================================

    FaustFileFilter.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#ifndef FAUSTFILEFILTER_H_INCLUDED
#define FAUSTFILEFILTER_H_INCLUDED

#include "JuceHeader.h"

class FaustDSPFileFilter : public FileFilter
{
public:
  FaustDSPFileFilter()
  : FileFilter("Faust DSP file")
  {
  }
  
  ~FaustDSPFileFilter()
  {
  }
  
  bool isFileSuitable (const File &file) const override
  {
    //todo contains process
    if(file.getFileExtension() == ".dsp")
      return true;
    else
      return false;
  }
  
  bool isDirectorySuitable (const File &file) const override
  {
    return false;
  }
};

class FaustLIBFileFilter : public FileFilter
{
public:
  FaustLIBFileFilter()
  : FileFilter("Faust library file")
  {
  }
  
  ~FaustLIBFileFilter()
  {
  }
  
  bool isFileSuitable (const File &file) const override
  {
    if(file.getFileExtension() == ".lib")
      return true;
    else
      return false;
  }
  
  bool isDirectorySuitable (const File &file) const override
  {
    return false;
  }
};



#endif  // FAUSTFILEFILTER_H_INCLUDED
