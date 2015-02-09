/*
  ==============================================================================

    FaustGenFactory.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#ifndef FAUSTGENFACTORY_H_INCLUDED
#define FAUSTGENFACTORY_H_INCLUDED

#include <iostream>
#include <fstream>
#include <sstream>
//#include <string>
#include <set>
#include <vector>
#include <map>

#include "base64.h"
#include "faust/llvm-dsp.h"
#include "JuceHeader.h"

class FaustAudioProcessor;

using namespace std;

class faustgen_factory
{
  typedef vector<String>::const_iterator StringVectorIt;
  
private:
  
  set<FaustAudioProcessor*> fInstances;      // set of all DSP
  llvm_dsp_factory* fDSPfactory;  // pointer to the LLVM Faust factory

  String fSourceCode;
  String fBitCode;
  
  vector<String> fLibraryPath;    // path towards the Faust libraries
  String fDrawPath;               // path where to put SVG files
  
  vector<String> fOptions;        // options set in the 'compileoptions' message
  
  int fFaustNumber;               // faustgen object's number inside the patcher
  
  FaustAudioProcessor* fUpdateInstance;      // the instance that inited an update
  
  String fName;                   // name of the DSP group
  String fJSON;                   // JSON
  
  CriticalSection fDSPMutex;    // mutex to protect RT audio thread when recompiling DSP
  
  vector<String> fCompileOptions; // Faust compiler options

  void add_library_path(const String& library_path);
  void add_compile_option(const String& key, const String& value);
  void add_compile_option(const String& value);
  
public:
  
  faustgen_factory(const String& name);
  
  ~faustgen_factory();
  
  llvm_dsp_factory* create_factory_from_bitcode();
  llvm_dsp_factory* create_factory_from_sourcecode(FaustAudioProcessor* instance);
  llvm_dsp* create_dsp_aux(FaustAudioProcessor* instance);
  
  void free_dsp_factory();
  
  void default_compile_options();
  void print_compile_options();
  
//  void getfromdictionary(t_dictionary* d);
//  void appendtodictionary(t_dictionary* d);
  
  int get_number() { return fFaustNumber; }
  String get_name() { return fName; }
  
//  void read(File path);
//  void write(File path);
  
  String get_sourcecode() { return fSourceCode; }
//
//  const char* get_json() { return fJSON.c_str(); }
//  
  void update_sourcecode(int size, String source_code, FaustAudioProcessor* instance);
    
  // Compile DSP with -svg option and display the SVG files
  bool try_open_svg();
  void open_svg();
  void remove_svg();
  void display_svg();
  String get_svg_path();
  
  void add_instance(FaustAudioProcessor* dsp) { fInstances.insert(dsp); }
  void remove_instance(FaustAudioProcessor* dsp)
  {
    fInstances.erase(dsp);
    
    // Last instance : remove factory from global table and commit suicide...
    if (fInstances.size() == 0)
    {
      gFactoryMap.erase(fName);
      delete this;
    }
  }
  
  bool try_lock();
  bool lock();
  void unlock();
  
  static int gFaustCounter;       // global variable to count the number of faustgen objects inside the patcher
  
  static map<String, faustgen_factory*> gFactoryMap;
};


#endif  // FAUSTGENFACTORY_H_INCLUDED
