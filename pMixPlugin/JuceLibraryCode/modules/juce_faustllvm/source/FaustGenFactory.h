/*
  ==============================================================================

    FaustGenFactory.h
    Based on Faustgen~ by GRAME
    Author:  Oliver Larkin
 
 
    Faustgen~ license:
    ***********************************************************************
    FAUST Architecture File
    Copyright (C) 2010-2015 GRAME, Centre National de Creation Musicale
    ---------------------------------------------------------------------
    This Architecture section is free software; you can redistribute it
    and/or modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 3 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; If not, see <http://www.gnu.org/licenses/>.

    EXCEPTION : As a special exception, you may create a larger work
    that contains this FAUST architecture section and distribute
    that work under terms of your choice, so long as this FAUST
    architecture section is not modified.


    ************************************************************************
    ***********************************************************************

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

using namespace juce;

class FaustAudioPluginInstance;

class FaustgenFactory
{  
private:
  
  std::set<FaustAudioPluginInstance*> fInstances;      // set of all DSP
  llvm_dsp_factory* fDSPfactory;  // pointer to the LLVM Faust factory

  String fSourceCode;
  String fBitCode;
  
  FileSearchPath fLibraryPath;    // paths to search for the Faust libraries
  String fDrawPath;               // path where to put SVG files
  
  StringArray fExtraOptions;
  
  int fFaustNumber;               // faustgen object's number inside the patcher
  
  FaustAudioPluginInstance* fUpdateInstance;      // the instance that inited an update
  
  String fName;                   // name of the DSP group
  String fJSON;              // JSON
  
  StringArray fCompileOptions; // Faust compiler options

  void addCompileOption(const String& key, const String& value);
  void addCompileOption(const String& value);
  
public:
  CriticalSection fDSPMutex;    // mutex to protect RT audio thread when recompiling DSP

  FaustgenFactory(const String& name, const File& path);
  ~FaustgenFactory();
  
  llvm_dsp_factory* createFactoryFromBitcode();
  llvm_dsp_factory* createFactoryFromSourceCode(FaustAudioPluginInstance* instance);
  llvm_dsp* createDSPAux(FaustAudioPluginInstance* instance);
  
  void freeDSPFactory();
  
  void defaultCompileOptions();
  void printCompileOptions();
  
  void getStateInformation (XmlElement& xml);
  void setStateInformation (XmlElement& xml);
  
  int getNumber() { return fFaustNumber; }
  String getName() { return fName; }
  
  void addLibraryPath(const File& libraryPath);
  
  String getSourcecode() { return fSourceCode; }

  String getJSON() { return fJSON; }
  
  void updateSourceCode(String sourceCode, FaustAudioPluginInstance* instance);
    
  // Compile DSP with -svg option and display the SVG files
  bool tryOpenSVG();
  void openSVG();
  void removeSVG();
  void displaySVG();
  String getSVGPath();
  
  void addInstance(FaustAudioPluginInstance* dsp) { fInstances.insert(dsp); }
  void removeInstance(FaustAudioPluginInstance* dsp)
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
