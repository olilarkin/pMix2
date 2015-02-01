/*
  ==============================================================================

    FaustGenFactory.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "FaustGenFactory.h"
#include "FaustAudioProcessor.h"
#include <stdio.h>

int faustgen_factory::gFaustCounter = 0;
map<string, faustgen_factory*> faustgen_factory::gFactoryMap;

//===================
// Faust DSP Factory
//===================

#ifdef __APPLE__
static string getTarget()
{
  int tmp;
  return (sizeof(&tmp) == 8) ? "" : "i386-apple-darwin10.6.0";
}
#else
static string getTarget() { return ""; }
#endif

//static string getFolderFromFilename(const string& fullpath)
//{
//  size_t first = fullpath.find_first_of(SEPARATOR);
//  size_t last = fullpath.find_last_of(SEPARATOR);
//  return (first != string::npos && last != string::npos) ? fullpath.substr(first, last - first) : "";
//}
//
//static string getFolderFromPath(const string& fullpath)
//{
//  size_t first = fullpath.find_first_of(SEPARATOR);
//  return (first != string::npos) ? fullpath.substr(first, fullpath.size() - first) : "";
//}
//
//struct Max_Meta : public Meta
//{
//  void declare(const char* key, const char* value)
//  {
//    if ((strcmp("name", key) == 0) || (strcmp("author", key) == 0)) {
//      printf("%s : %s", key, value);
//    }
//  }
//};

faustgen_factory::faustgen_factory(const string& name)
{
  fUpdateInstance = 0;
  fName = name;
  fDSPfactory = 0;
  fBitCodeSize = 0;
  fBitCode = 0;
  fSourceCodeSize = 0;
  fSourceCode = 0;
  gFaustCounter++;
  fFaustNumber = gFaustCounter;
  
//#ifdef __APPLE__
//  // OSX only : access to the fautgen~ bundle
//  CFBundleRef faustgen_bundle = CFBundleGetBundleWithIdentifier(CFSTR("com.grame.faustgen"));
//  CFURLRef faustgen_ref = CFBundleCopyBundleURL(faustgen_bundle);
//  UInt8 bundle_path[512];
//  Boolean res = CFURLGetFileSystemRepresentation(faustgen_ref, true, bundle_path, 512);
//  assert(res);
//  
//  // Built the complete resource path
//  fLibraryPath.push_back(string((const char*)bundle_path) + string(FAUST_LIBRARY_PATH));
//  
//  // Draw path in temporary folder
//  fDrawPath = string(FAUST_DRAW_PATH);
//#endif
//  
//#ifdef WIN32
//  HMODULE handle = LoadLibrary("faustgen~.mxe");
//  if (handle) {
//    // Get faustgen~.mxe path
//    char name[512];
//    GetModuleFileName(handle, name, 512);
//    string str_name = string(name);
//    str_name = str_name.substr(0, str_name.find_last_of("\\"));
//    // Built the complete resource path
//    fLibraryPath.push_back(string(str_name) + string(FAUST_LIBRARY_PATH));
//    // Draw path in temporary folder
//    TCHAR lpTempPathBuffer[MAX_PATH];
//    // Gets the temp path env string (no guarantee it's a valid path).
//    DWORD dwRetVal = GetTempPath(MAX_PATH, lpTempPathBuffer);
//    if (dwRetVal > MAX_PATH || (dwRetVal == 0)) {
//      printf("GetTempPath failed...");
//      // Try our value instead...
//      fDrawPath = string(str_name) + string(FAUST_DRAW_PATH);
//    } else {
//      fDrawPath = string(lpTempPathBuffer);
//    }
//    FreeLibrary(handle);
//  } else {
//    printf("Error : cannot locate faustgen~.mxe...");
//    fDrawPath = "";
//  }
//#endif
  
//  t_max_err err = systhread_mutex_new(&fDSPMutex, SYSTHREAD_MUTEX_NORMAL);
//  if (err != MAX_ERR_NONE) {
//    printf("Cannot allocate mutex...");
//  }
}

faustgen_factory::~faustgen_factory()
{
//  free_dsp_factory();
//  free_sourcecode();
//  free_bitcode();
//  
//  remove_svg();
//  systhread_mutex_free(fDSPMutex);
}

//void faustgen_factory::free_sourcecode()
//{
//  if (fSourceCode) {
////    sysmem_freehandle(fSourceCode);
//    fSourceCodeSize = 0;
//    fSourceCode = 0;
//  }
//}
//
//void faustgen_factory::free_bitcode()
//{
//  if (fBitCode) {
////    sysmem_freehandle(fBitCode);
//    fBitCodeSize = 0;
//    fBitCode = 0;
//  }
//}
//
//void faustgen_factory::free_dsp_factory()
//{
//  if (lock()) {
//    
//    // Free all instances
//    set<FaustAudioProcessor*>::const_iterator it;
//    for (it = fInstances.begin(); it != fInstances.end(); it++) {
//      (*it)->free_dsp();
//    }
//    
//    //deleteDSPFactory(fDSPfactory);
//    fDSPfactory = 0;
//    unlock();
//  } else {
//    printf("Mutex lock cannot be taken...");
//  }
//}

//llvm_dsp_factory* faustgen_factory::create_factory_from_bitcode()
//{
//  string decoded_bitcode = base64_decode(*fBitCode, fBitCodeSize);
//  return readDSPFactoryFromBitcode(decoded_bitcode, getTarget(), LLVM_OPTIMIZATION);
  
  /*
   // Alternate model using machine code
   return readDSPFactoryFromMachine(decoded_bitcode);
   */
  
  /*
   // Alternate model using LLVM IR
   return readDSPFactoryFromIR(*fBitCode, getTarget(), LLVM_OPTIMIZATION);
   */
//}

llvm_dsp_factory* faustgen_factory::create_factory_from_sourcecode(FaustAudioProcessor* instance)
{
  char name_app[64];
  sprintf(name_app, "faustgen-%d", fFaustNumber);
  
  // To be sure we get a correct SVG diagram...
//  remove_svg();
//  
//  default_compile_options();
//  print_compile_options();
  
  // Prepare compile options
  string error;
 	const char* argv[64];
  
  assert(fCompileOptions.size() < 64);
  StringVectorIt it;
  int i = 0;
  for (it = fCompileOptions.begin(); it != fCompileOptions.end(); it++) {
    argv[i++] = (char*)(*it).c_str();
  }
  
  // Generate SVG file
  if (!generateAuxFilesFromString(name_app, *fSourceCode, fCompileOptions.size(), argv, error)) {
    printf("Generate SVG error : %s", error.c_str());
  }
  
  llvm_dsp_factory* factory = createDSPFactoryFromString(name_app, *fSourceCode, fCompileOptions.size(), argv, getTarget(), error, LLVM_OPTIMIZATION);
  
  if (factory)
  {
    return factory;
  }
  else
  {
//    if (fUpdateInstance == instance)
//    {
//      instance->hilight_on(error);
//    }
    printf("Invalid Faust code or compile options : %s", error.c_str());
    return 0;
  }
}

llvm_dsp* faustgen_factory::create_dsp_aux(FaustAudioProcessor* instance)
{
  llvm_dsp* dsp = 0;
//  Max_Meta meta;
  string error;
  
  // Factory already allocated
  if (fDSPfactory)
  {
    dsp = createDSPInstance(fDSPfactory);
    printf("Factory already allocated, %i input(s), %i output(s)", dsp->getNumInputs(), dsp->getNumOutputs());
    goto end;
  }
  
  // Tries to create from bitcode
//  if (fBitCodeSize > 0) {
//    fDSPfactory = create_factory_from_bitcode();
//    if (fDSPfactory) {
//      metadataDSPFactory(fDSPfactory, &meta);
//      dsp = createDSPInstance(fDSPfactory);
//      printf("Compilation from bitcode succeeded, %i input(s), %i output(s)", dsp->getNumInputs(), dsp->getNumOutputs());
//      goto end;
//    }
//  }
  
  // Otherwise tries to create from source code
  if (fSourceCodeSize > 0)
  {
    fDSPfactory = create_factory_from_sourcecode(instance);
    if (fDSPfactory)
    {
//      metadataDSPFactory(fDSPfactory, &meta);
      dsp = createDSPInstance(fDSPfactory);
      printf("Compilation from source code succeeded, %i input(s), %i output(s)", dsp->getNumInputs(), dsp->getNumOutputs());
      goto end;
    }
  }
  
  // Otherwise creates default DSP keeping the same input/output number
  fDSPfactory = createDSPFactoryFromString("default", DEFAULT_CODE, 0, 0, getTarget(), error, LLVM_OPTIMIZATION);
  dsp = createDSPInstance(fDSPfactory);
  printf("Allocation of default DSP succeeded, %i input(s), %i output(s)", dsp->getNumInputs(), dsp->getNumOutputs());
  
end:
  
  assert(dsp);

  // Prepare JSON
//  JSONUI builder(m_siginlets, m_sigoutlets);
//  metadataDSPFactory(fDSPfactory, &builder);
//  dsp->buildUserInterface(&builder);
//  fJSON = builder.JSON();
  
  return dsp;
}

//void faustgen_factory::add_library_path(const string& library_path)
//{
//  if ((library_path != "") && find(fLibraryPath.begin(), fLibraryPath.end(), library_path) == fLibraryPath.end()) {
//    fLibraryPath.push_back(library_path);
//  }
//}
//
//void faustgen_factory::add_compile_option(const string& key, const string& value)
//{
//  if ((value != "") && find(fCompileOptions.begin(), fCompileOptions.end(), value) == fCompileOptions.end()) {
//    fCompileOptions.push_back(key);
//    fCompileOptions.push_back(value);
//  }
//}
//
//void faustgen_factory::add_compile_option(const string& value)
//{
//  if ((value != "") && find(fCompileOptions.begin(), fCompileOptions.end(), value) == fCompileOptions.end()) {
//    fCompileOptions.push_back(value);
//  }
//}
//
//void faustgen_factory::print_compile_options()
//{
//  if (fCompileOptions.size() > 0) {
//    printf("-----------------------------");
//    StringVectorIt it;
//    for (it = fCompileOptions.begin(); it != fCompileOptions.end(); it++) {
//      printf("Compile option = %s", (*it).c_str());
//    }
//    printf("-----------------------------");
//  }
//}

//void faustgen_factory::default_compile_options()
//{
//  // Clear and set default value
//  fCompileOptions.clear();
//  
//  // By default when double
//  if (sizeof(FAUSTFLOAT) == 8) {
//    add_compile_option("-double");
//  }
//  
//  // Add -svg to current compile options
//  add_compile_option("-svg");
//  
//  // All library paths
//  StringVectorIt it;
//  for (it = fLibraryPath.begin(); it != fLibraryPath.end(); it++) {
//    add_compile_option("-I", *it);
//  }
//  
//  // Draw path
//  add_compile_option("-O", fDrawPath);
//  
//  // All options set in the 'compileoptions' message
//  for (it = fOptions.begin(); it != fOptions.end(); it++) {
//    add_compile_option(*it);
//  }
//  
//  // Vector mode by default
//  /*
//   add_compile_option("-vec");
//   add_compile_option("-lv");
//   add_compile_option("1");
//   */
//  /*
//   Seems not necessary...
//   fCompileOptions.push_back("-vs");
//   stringstream num;
//   num << sys_getblksize();
//   add_compile_option(num.str());
//   */
//}

//void faustgen_factory::getfromdictionary(t_dictionary* d)
//{
//  // Read sourcecode "faustgen_version" key
//  const char* faustgen_version;
//  t_max_err err = dictionary_getstring(d, gensym("version"), &faustgen_version);
//  
//  if (err != MAX_ERR_NONE) {
//    printf("Cannot read \"version\" key, so ignore bitcode, force recompilation and use default compileoptions");
//    goto read_sourcecode;
//  } else if (strcmp(faustgen_version, FAUSTGEN_VERSION) != 0) {
//    printf("Older version of faustgen~ (%s versus %s), so ignore bitcode, force recompilation and use default compileoptions", FAUSTGEN_VERSION, faustgen_version);
//    goto read_sourcecode;
//  }
//  
//  // Read bitcode size key
//  err = dictionary_getlong(d, gensym("bitcode_size"), (t_atom_long*)&fBitCodeSize);
//  if (err != MAX_ERR_NONE) {
//    fBitCodeSize = 0;
//    goto read_sourcecode;
//  }
//  
//  // If OK read bitcode
//  
//  //printf("read bitcode fBitCodeSize %d\n", fBitCodeSize);
//  
//  fBitCode = sysmem_newhandleclear(fBitCodeSize + 1);             // We need to use a size larger by one for the null terminator
//  const char* bitcode;
//  err = dictionary_getstring(d, gensym("bitcode"), &bitcode);     // The retrieved pointer references the string in the dictionary, it is not a copy.
//  sysmem_copyptr(bitcode, *fBitCode, fBitCodeSize);
//  if (err != MAX_ERR_NONE) {
//    fBitCodeSize = 0;
//  }
//  
//  //printf("read bitcode fBitCodeSize OK %d\n", fBitCodeSize);
//  
//read_sourcecode:
//  
//  // Read sourcecode size key
//  err = dictionary_getlong(d, gensym("sourcecode_size"), (t_atom_long*)&fSourceCodeSize);
//  if (err != MAX_ERR_NONE) {
//    goto default_sourcecode;
//  }
//  
//  // If OK read sourcecode
//  fSourceCode = sysmem_newhandleclear(fSourceCodeSize + 1);           // We need to use a size larger by one for the null terminator
//  const char* sourcecode;
//  err = dictionary_getstring(d, gensym("sourcecode"), &sourcecode);   // The retrieved pointer references the string in the dictionary, it is not a copy.
//  sysmem_copyptr(sourcecode, *fSourceCode, fSourceCodeSize);
//  if (err == MAX_ERR_NONE) {
//    return;
//  }
//  
//default_sourcecode:
//  
//  // Otherwise tries to create from default source code
//  fSourceCodeSize = strlen(DEFAULT_SOURCE_CODE);
//  fSourceCode = sysmem_newhandleclear(fSourceCodeSize + 1);
//  sysmem_copyptr(DEFAULT_SOURCE_CODE, *fSourceCode, fSourceCodeSize);
//}

// Called when saving the Max patcher
// This function saves the necessary data inside the json file (Faust sourcecode)
//void faustgen_factory::appendtodictionary(t_dictionary* d)
//{
//  printf("Saving object version, sourcecode and bitcode...");
//  
//  // Save faustgen~ version
//  dictionary_appendstring(d, gensym("version"), FAUSTGEN_VERSION);
//  
//  // Save source code
//  if (fSourceCodeSize) {
//    dictionary_appendlong(d, gensym("sourcecode_size"), fSourceCodeSize);
//    dictionary_appendstring(d, gensym("sourcecode"), *fSourceCode);
//  }
//  
//  // Save bitcode
//  if (fDSPfactory) {
//    string bitcode = writeDSPFactoryToBitcode(fDSPfactory);
//    
//    // Alternate model using LLVM IR
//    // string ircode = writeDSPFactoryToIR(fDSPfactory);
//    
//    // Alternate model using machine code
//    //string machinecode = writeDSPFactoryToMachine(fDSPfactory);
//    
//    string encoded_bitcode = base64_encode((const unsigned char*)bitcode.c_str(), bitcode.size());
//    dictionary_appendlong(d, gensym("bitcode_size"), encoded_bitcode.size());
//    dictionary_appendstring(d, gensym("bitcode"), encoded_bitcode.c_str());
//  }
//}

//bool faustgen_factory::try_open_svg()
//{
//  // Open the svg diagram file inside a web browser
//  char command[512];
//#ifdef WIN32
//  sprintf(command, "type \"file:///%sfaustgen-%d-svg/process.svg\"", fDrawPath.c_str(), fFaustNumber);
//#else
//  sprintf(command, "open -a Safari \"file://%sfaustgen-%d-svg/process.svg\"", fDrawPath.c_str(), fFaustNumber);
//#endif
//  return (system(command) == 0);
//}
//
//void faustgen_factory::open_svg()
//{
//  // Open the svg diagram file inside a web browser
//  char command[512];
//#ifdef WIN32
//  sprintf(command, "start \"\" \"file:///%sfaustgen-%d-svg/process.svg\"", fDrawPath.c_str(), fFaustNumber);
//#else
//  sprintf(command, "open -a Safari \"file://%sfaustgen-%d-svg/process.svg\"", fDrawPath.c_str(), fFaustNumber);
//#endif
//  //printf("open_svg %s", command);
//  system(command);
//}
//
//void faustgen_factory::remove_svg()
//{
//  // Possibly done by "compileoptions" or display_svg
//  char command[512];
//#ifdef WIN32
//  sprintf(command, "rmdir /S/Q \"%sfaustgen-%d-svg\"", fDrawPath.c_str(), fFaustNumber);
//#else
//  sprintf(command, "rm -r \"%sfaustgen-%d-svg\"", fDrawPath.c_str(), fFaustNumber);
//#endif
//  system(command);
//}
//
//void faustgen_factory::display_svg()
//{
//  // Try to open SVG svg diagram file inside a web browser
//  if (!try_open_svg()) {
//    
//    printf("SVG diagram not available, recompile to produce it");
//    
//    // Force recompilation to produce it
//    llvm_dsp_factory* factory = create_factory_from_sourcecode(0);
//    //deleteDSPFactory(factory);
//    
//    // Open the SVG diagram file inside a web browser
//    open_svg();
//  }
//}
//
//bool faustgen_factory::open_file(const char* file)
//{
//  char command[512];
//#ifdef WIN32
//  sprintf(command, "start \"\" \"%s%s\"", (*fLibraryPath.begin()).c_str(), file);
//#else
//  sprintf(command, "open \"%s%s\"", (*fLibraryPath.begin()).c_str(), file);
//#endif
//  printf(command);
//  return (system(command) == 0);
//}
//
//bool faustgen_factory::open_file(const char* appl, const char* file)
//{
//  char command[512];
//#ifdef WIN32
//  sprintf(command, "start \"\" %s \"%s%s\"", appl, (*fLibraryPath.begin()).c_str(), file);
//#else
//  sprintf(command, "open -a %s \"%s%s\"", appl, (*fLibraryPath.begin()).c_str(), file);
//#endif
//  return (system(command) == 0);
//}
//
//void faustgen_factory::display_pdf()
//{
//  // Open the PDF documentation
//  open_file(FAUST_PDF_DOCUMENTATION);
//}
//
//void faustgen_factory::display_libraries_aux(const char* lib)
//{
//  const char* appl;
//  int i = 0;
//  
//  while ((appl = TEXT_APPL_LIST[i++]) && (strcmp(appl, "") != 0)) {
//    if (open_file(appl, lib)) {
//      break;
//    }
//  }
//}
//
//void faustgen_factory::display_libraries()
//{
//  // Open the libraries
//#ifdef WIN32
//  open_file("effect.lib");
//  open_file("filter.lib");
//  open_file("math.lib");
//  open_file("maxmsp.lib");
//  open_file("music.lib");
//  open_file("oscillator.lib");
//  open_file("reduce.lib");
//#else
//  display_libraries_aux("effect.lib");
//  display_libraries_aux("filter.lib");
//  display_libraries_aux("math.lib");
//  display_libraries_aux("maxmsp.lib");
//  display_libraries_aux("music.lib");
//  display_libraries_aux("oscillator.lib");
//  display_libraries_aux("reduce.lib");
//#endif
//}

void faustgen_factory::update_sourcecode(int size, char* source_code, FaustAudioProcessor* instance)
{
  // Recompile only if text has been changed
  if (strcmp(source_code, *fSourceCode) != 0) {
    
    // Update all instances
    set<FaustAudioProcessor*>::const_iterator it;
//    for (it = fInstances.begin(); it != fInstances.end(); it++) {
//      (*it)->hilight_off();
//    }
    
    // Delete the existing Faust module
//    free_dsp_factory();
//    
//    // Free the memory allocated for fSourceCode
//    free_sourcecode();
//    
//    // Free the memory allocated for fBitCode
//    free_bitcode();
//    
//    // Allocate the right memory for fSourceCode
//    fSourceCode = sysmem_newhandleclear(size + 1);
//    sysmem_copyptr(source_code, *fSourceCode, size);
//    fSourceCodeSize = size;
//    
//    // Update all instances
//    fUpdateInstance = instance;
//    for (it = fInstances.begin(); it != fInstances.end(); it++) {
//      (*it)->update_sourcecode();
//    }
//    
  } else {
    printf("DSP code has not been changed...");
  }
}

//void faustgen_factory::librarypath(long inlet, t_symbol* s)
//{
//  if (s != gensym("")) {
//    add_library_path(getFolderFromPath(s->s_name));
//  }
//}

//void faustgen_factory::read(long inlet, t_symbol* s)
//{
//  char filename[MAX_FILENAME_CHARS];
//  short path = 0;
//  long type = 'TEXT';
//  short err;
//  t_filehandle fh;
//  
//  // No filename, so open load dialog
//  if (s == gensym("")) {
//    filename[0] = 0;
//    if (open_dialog(filename, &path, (t_fourcc*)&type, (t_fourcc*)&type, 1)) {
//      printf("Faust DSP file not found");
//      return;
//    }
//    // Otherwise locate the file
//  } else {
//    strcpy(filename, s->s_name);
//    if (locatefile_extended(filename, &path, (t_fourcc*)&type, (t_fourcc*)&type, 1)) {
//      printf("Faust DSP file '%s' not found", filename);
//      return;
//    }
//  }
//  
//  // File found, open it and recompile DSP
//  err = path_opensysfile(filename, path, &fh, READ_PERM);
//  if (err) {
//    printf("Faust DSP file '%s' cannot be opened", filename);
//    return;
//  }
//  
//  // Delete the existing Faust module
//  free_dsp_factory();
//  
//  // Free the memory allocated for fBitCode
//  free_bitcode();
//  
//  err = sysfile_readtextfile(fh, fSourceCode, 0, (t_sysfile_text_flags)(TEXT_LB_UNIX | TEXT_NULL_TERMINATE));
//  if (err) {
//    printf("Faust DSP file '%s' cannot be read", filename);
//  }
//  
//  sysfile_close(fh);
//  fSourceCodeSize = sysmem_handlesize(fSourceCode);
//  
//  // Add DSP file enclosing folder pathname in the '-I' list
//  char full_path[MAX_FILENAME_CHARS];
//  if (path_topathname(path, filename, full_path) == 0) {
//    add_library_path(getFolderFromFilename(full_path));
//  }
//  
//  // Update all instances
//  set<faustgen*>::const_iterator it;
//  for (it = fInstances.begin(); it != fInstances.end(); it++) {
//    (*it)->update_sourcecode();
//  }
//}

//void faustgen_factory::write(long inlet, t_symbol* s)
//{
//  char filename[MAX_FILENAME_CHARS];
//  short path = 0;
//  long type = 'TEXT';
//  short err;
//  t_filehandle fh;
//  
//  // No filename, so open save dialog
//  if (s == gensym("")) {
//    filename[0] = 0;
//    if (saveas_dialog(filename, &path, NULL)) {
//      printf("Faust DSP file not found");
//      return;
//    } else {
//      err = path_createsysfile(filename, path, type, &fh);
//      if (err) {
//        printf("Faust DSP file '%s' cannot be created", filename);
//        return;
//      }
//    }
//    // Otherwise locate or create the file
//  } else {
//    strcpy(filename, s->s_name);
//    if (locatefile_extended(filename, &path, (t_fourcc*)&type, (t_fourcc*)&type, 1)) {
//      printf("Faust DSP file '%s' not found, so tries to create it", filename);
//      err = path_createsysfile(filename, path, type, &fh);
//      if (err) {
//        printf("Faust DSP file '%s' cannot be created", filename);
//        return;
//      }
//    } else {
//      err = path_opensysfile(filename, path, &fh, WRITE_PERM);
//      if (err) {
//        printf("Faust DSP file '%s' cannot be opened", filename);
//        return;
//      }
//    }
//  }
//  
//  err = sysfile_writetextfile(fh, fSourceCode, (t_sysfile_text_flags)(TEXT_LB_UNIX | TEXT_NULL_TERMINATE));
//  if (err) {
//    printf("Faust DSP file '%s' cannot be written", filename);
//  }
//  sysfile_close(fh);
//}

//void faustgen_factory::compileoptions(long inlet, t_symbol* s, long argc, t_atom* argv)
//{
//  printf("Compiler options modified for faustgen");
//  
//  if (argc == 0) {
//    printf("No argument entered, no additional compilation option will be used");
//  }
//  
//  // Clear options
//  fOptions.clear();
//  bool optimize = false;
//  int i;
//  t_atom* ap;
//  
//  // Increment ap each time to get to the next atom
//  for (i = 0, ap = argv; i < argc; i++, ap++) {
//    switch (atom_gettype(ap)) {
//      case A_LONG: {
//        stringstream num;
//        num << atom_getlong(ap);
//        string res = num.str();
//        fOptions.push_back(res.c_str());
//        break;
//      }
//      case A_FLOAT:
//        printf("Invalid compiler option argument - float");
//        break;
//      case A_SYM:
//        // Add options to default ones
//        if (strcmp("-opt", atom_getsym(ap)->s_name) == 0) {
//          optimize = true;
//        } else {
//          fOptions.push_back(atom_getsym(ap)->s_name);
//        }
//        break;
//      default:
//        printf("Invalid compiler option argument - unknown");
//        break;
//    }
//  }
//  
//  if (optimize) {
//    
//    printf("Start looking for optimal compilation options...");
//    
//#ifndef WIN32
//    FaustLLVMOptimizer optimizer(string(*fSourceCode), (*fLibraryPath.begin()).c_str(), getTarget(), 2000, sys_getblksize());
//    fOptions = optimizer.findOptimize();
//#endif
//    
//    printf("Optimal compilation options found");
//  }
//  
//  // Delete the existing Faust module
//  free_dsp_factory();
//  
//  // Free the memory allocated for fBitCode
//  free_bitcode();
//  
//  // Update all instances
//  set<faustgen*>::const_iterator it;
//  for (it = fInstances.begin(); it != fInstances.end(); it++) {
//    (*it)->update_sourcecode();
//  }
//}