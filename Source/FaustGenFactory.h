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
#include <string>
#include <set>
#include <vector>
#include <map>

#include "faust/llvm-dsp.h"

class FaustAudioProcessor;

using namespace std;

class faustgen_factory
{
  typedef vector<string>::const_iterator StringVectorIt;
  
private:
  
  set<FaustAudioProcessor*> fInstances;      // set of all DSP
  llvm_dsp_factory* fDSPfactory;  // pointer to the LLVM Faust factory
  
  long fSourceCodeSize;           // length of source code string
  char** fSourceCode;             // source code string
  
  long fBitCodeSize;              // length of the bitcode string
  char** fBitCode;                // bitcode string
  
  vector<string> fLibraryPath;    // path towards the Faust libraries
  string fDrawPath;               // path where to put SVG files
  
  vector<string> fOptions;        // options set in the 'compileoptions' message
  
  int fFaustNumber;               // faustgen object's number inside the patcher
  
  faustgen_factory* fUpdateInstance;      // the instance that inited an update
  
  string fName;                   // name of the DSP group
  string fJSON;                   // JSON
  
//  t_systhread_mutex fDSPMutex;    // mutex to protect RT audio thread when recompiling DSP
  
  vector<string> fCompileOptions; // Faust compiler options

//  bool open_file(const char* file);
//  bool open_file(const char* appl, const char* file);
//  
//  void add_library_path(const string& library_path);
//  void add_compile_option(const string& key, const string& value);
//  void add_compile_option(const string& value);
  
public:
  
  faustgen_factory(const string& name);
  
  ~faustgen_factory();
  
  //llvm_dsp_factory* create_factory_from_bitcode();
  llvm_dsp_factory* create_factory_from_sourcecode(FaustAudioProcessor* instance);
  llvm_dsp* create_dsp_aux(FaustAudioProcessor* instance);
  
//  void free_dsp_factory();
//  void free_sourcecode();
//  void free_bitcode();
//  
//  void default_compile_options();
//  void print_compile_options();
  
//  void getfromdictionary(t_dictionary* d);
//  void appendtodictionary(t_dictionary* d);
  
//  int get_number() { return fFaustNumber; }
//  string get_name() { return fName; }
  
//  void read(long inlet, t_symbol* s);
//  void write(long inlet, t_symbol* s);
//  void librarypath(long inlet, t_symbol* s);
  
//  char* get_sourcecode() { return *fSourceCode; }
//  
//  const char* get_json() { return fJSON.c_str(); }
//  
  void update_sourcecode(int size, char* source_code, FaustAudioProcessor* instance);
  
//  void compileoptions(long inlet, t_symbol* s, long argc, t_atom* argv);
  
  // Compile DSP with -svg option and display the SVG files
//  bool try_open_svg();
//  void open_svg();
//  void remove_svg();
//  void display_svg();
//  void display_pdf();
//  void display_libraries();
//  void display_libraries_aux(const char* lib);
  
  void add_instance(FaustAudioProcessor* dsp) { fInstances.insert(dsp); }
  void remove_instance(FaustAudioProcessor* dsp)
  {
    fInstances.erase(dsp);
    
    // Last instance : remove factory from global table and commit suicide...
    if (fInstances.size() == 0) {
      gFactoryMap.erase(fName);
      delete this;
    }
  }
  
  //TODO:
  bool try_lock() { return true; }
  bool lock() { return true; }
  void unlock() { return; }
  
  static int gFaustCounter;       // global variable to count the number of faustgen objects inside the patcher
  
  static map<string, faustgen_factory*> gFactoryMap;
};


#endif  // FAUSTGENFACTORY_H_INCLUDED
