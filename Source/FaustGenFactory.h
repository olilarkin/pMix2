/*
  ==============================================================================

    FaustGenFactory.h
    Based on Faustgen~ by GRAME
    Author:  Oliver Larkin

  ==============================================================================
*/

#ifndef FAUSTGENFACTORY_H_INCLUDED
#define FAUSTGENFACTORY_H_INCLUDED

#include <iostream>
#include <fstream>

#include <set>
#include <vector>
#include <map>
#include <string.h>

#include "faust/gui/JSONUI.h"
#include "faust/llvm-dsp.h"
#include "JuceHeader.h"

class FaustAudioProcessor;

class FaustgenFactory
{
  typedef std::vector<String>::const_iterator StringVectorIt;
  
private:
  
  std::set<FaustAudioProcessor*> fInstances;      // set of all DSP
  llvm_dsp_factory* fDSPfactory;  // pointer to the LLVM Faust factory

  String fSourceCode;
  String fBitCode;
  
  std::vector<String> fLibraryPath;    // path towards the Faust libraries
  String fDrawPath;               // path where to put SVG files
  
  std::vector<String> fOptions;        // options set in the 'compileoptions' message
  
  int fFaustNumber;               // faustgen object's number inside the patcher
  
  FaustAudioProcessor* fUpdateInstance;      // the instance that inited an update
  
  String fName;                   // name of the DSP group
  String fJSON;              // JSON
  
  std::vector<String> fCompileOptions; // Faust compiler options

  void addCompileOption(const String& key, const String& value);
  void addCompileOption(const String& value);
  
public:
  CriticalSection fDSPMutex;    // mutex to protect RT audio thread when recompiling DSP

  FaustgenFactory(const String& name, const String& path);
  ~FaustgenFactory();
  
  llvm_dsp_factory* createFactoryFromBitcode();
  llvm_dsp_factory* createFactoryFromSourceCode(FaustAudioProcessor* instance);
  llvm_dsp* createDSPAux(FaustAudioProcessor* instance);
  
  void freeDSPFactory();
  
  void defaultCompileOptions();
  void printCompileOptions();
  
  void getStateInformation (XmlElement& xml);
  void setStateInformation (XmlElement& xml);
  
  int getNumber() { return fFaustNumber; }
  String getName() { return fName; }
  
  void addLibraryPath(const String& libraryPath);
  
//  void read(File path);
//  void write(File path);
  
  String getSourcecode() { return fSourceCode; }

  String getJSON() { return fJSON; }
  
  void updateSourceCode(String sourceCode, FaustAudioProcessor* instance);
    
  // Compile DSP with -svg option and display the SVG files
  bool tryOpenSVG();
  void openSVG();
  void removeSVG();
  void displaySVG();
  String getSVGPath();
  
  void addInstance(FaustAudioProcessor* dsp) { fInstances.insert(dsp); }
  void removeInstance(FaustAudioProcessor* dsp)
  {
    fInstances.erase(dsp);
    
    // Last instance : remove factory from global table and commit suicide...
    if (fInstances.size() == 0)
    {
      gFactoryMap.erase(fName);
      delete this;
    }
  }

  static int gFaustCounter;       // global variable to count the number of faustgen objects inside the patcher
  
  static std::map<String, FaustgenFactory*> gFactoryMap;
};


#endif  // FAUSTGENFACTORY_H_INCLUDED
