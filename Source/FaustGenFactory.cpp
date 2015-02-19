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
map<String, faustgen_factory*> faustgen_factory::gFactoryMap;

#if JUCE_MAC
static string getTarget()
{
  int tmp;
  return (sizeof(&tmp) == 8) ? "" : "i386-apple-darwin10.6.0";
}
#else
//#elif JUCE_WIN32
static string getTarget() { return ""; }
#endif

faustgen_factory::faustgen_factory(const String& name)
{
  fUpdateInstance = 0;
  fName = name;
  fDSPfactory = 0;
  fSourceCode = DEFAULT_CODE;
  gFaustCounter++;
  fFaustNumber = gFaustCounter;
  
#if JUCE_MAC
  
#if PMIX_PLUGIN
  CFBundleRef faustgen_bundle = CFBundleGetBundleWithIdentifier(CFSTR("com.OliLarkin.pMixPlugin"));
#else
  // OSX only : access to the pMix bundle
  CFBundleRef faustgen_bundle = CFBundleGetBundleWithIdentifier(CFSTR("com.OliLarkin.pMix"));
#endif
  CFURLRef faustgen_ref = CFBundleCopyBundleURL(faustgen_bundle);
  UInt8 bundle_path[512];
  Boolean res = CFURLGetFileSystemRepresentation(faustgen_ref, true, bundle_path, 512);
  jassert(res);
  
  // Built the complete resource path
  fLibraryPath.push_back(string((const char*)bundle_path) + string(FAUST_LIBRARY_PATH));
  
  // Draw path in temporary folder
  fDrawPath = string(FAUST_DRAW_PATH);
#endif
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
//      LOG("GetTempPath failed...");
//      // Try our value instead...
//      fDrawPath = string(str_name) + string(FAUST_DRAW_PATH);
//    } else {
//      fDrawPath = string(lpTempPathBuffer);
//    }
//    FreeLibrary(handle);
//  } else {
//    LOG("Error : cannot locate faustgen~.mxe...");
//    fDrawPath = "";
//  }
//#endif
}

faustgen_factory::~faustgen_factory()
{
  free_dsp_factory();
  remove_svg();
}

void faustgen_factory::free_dsp_factory()
{
  const ScopedLock lock(fDSPMutex);

  // Free all instances
  set<FaustAudioProcessor*>::const_iterator it;
  for (it = fInstances.begin(); it != fInstances.end(); it++) {
    (*it)->free_dsp();
  }

  //deleteDSPFactory(fDSPfactory); //commented out in faustgen~
  fDSPfactory = 0;
}

llvm_dsp_factory* faustgen_factory::create_factory_from_bitcode()
{
  return readDSPFactoryFromBitcode(fBitCode.toStdString(), getTarget(), LLVM_OPTIMIZATION);
  
  /*
    Alternate model using machine code
   return readDSPFactoryFromMachine(decoded_bitcode);
   */
  
  /*
    Alternate model using LLVM IR
   return readDSPFactoryFromIR(*fBitCode, getTarget(), LLVM_OPTIMIZATION);
   */
}

llvm_dsp_factory* faustgen_factory::create_factory_from_sourcecode(FaustAudioProcessor* instance)
{
  String name_app;
  name_app << "faustgen-" << fFaustNumber;
  
  // To be sure we get a correct SVG diagram...
  remove_svg();
  
  default_compile_options();
  //print_compile_options();
  
  // Prepare compile options
  string error;
 	const char* argv[64];
  
  jassert(fCompileOptions.size() < 64);
  StringVectorIt it;
  int i = 0;
  for (it = fCompileOptions.begin(); it != fCompileOptions.end(); it++)
  {
    argv[i++] = (char*)(*it).toRawUTF8();
  }
  
  // Generate SVG file
  if (!generateAuxFilesFromString(name_app.toStdString(), fSourceCode.toStdString() , fCompileOptions.size(), argv, error))
  {
    LOG("Generate SVG error : " + error);
  }
  
  // set "width" and "height" of svg to 100% so they fill web browser, TODO: improve this
  String path;
  path << "/" << fDrawPath << "faustgen-" << fFaustNumber << "-svg/process.svg";
  File svgFile(path);
  
  if (svgFile.exists())
  {
    XmlDocument svgXML(svgFile);
    ScopedPointer<XmlElement> mainElement (svgXML.getDocumentElement());
    mainElement->setAttribute("width", "100%");
    mainElement->setAttribute("height", "100%");
    mainElement->writeToFile(svgFile, String::empty);
  }
  
  llvm_dsp_factory* factory = createDSPFactoryFromString(name_app.toStdString(), fSourceCode.toStdString(), fCompileOptions.size(), argv, getTarget(), error, LLVM_OPTIMIZATION);
  
  if (factory)
  {
    return factory;
  }
  else
  {
    if (fUpdateInstance == instance)
    {
      instance->hilight_on(error);
    }
    LOG("Invalid Faust code or compile options : %s" + error);
    return 0;
  }
}

llvm_dsp* faustgen_factory::create_dsp_aux(FaustAudioProcessor* instance)
{
  llvm_dsp* dsp = 0;
  string error;
  String logStr;

  // Factory already allocated
  if (fDSPfactory)
  {
    dsp = createDSPInstance(fDSPfactory);
    logStr << "Factory already allocated, " <<  dsp->getNumInputs() << " input(s), " << dsp->getNumOutputs() << " output(s)";
    goto end;
  }
  
  // Tries to create from bitcode
  if (fBitCode.length() > 0)
  {
    fDSPfactory = create_factory_from_bitcode();
    if (fDSPfactory)
    {
      dsp = createDSPInstance(fDSPfactory);
      logStr << "Compilation from bitcode succeeded, " <<  dsp->getNumInputs() << " input(s), " << dsp->getNumOutputs() << " output(s)";
      goto end;
    }
  }
  
  // Otherwise tries to create from source code
  if (fSourceCode.length() > 0)
  {
    fDSPfactory = create_factory_from_sourcecode(instance);
    if (fDSPfactory)
    {
      dsp = createDSPInstance(fDSPfactory);
      logStr << "Compilation from source code succeeded, " <<  dsp->getNumInputs() << " input(s), " << dsp->getNumOutputs() << " output(s)";
      goto end;
    }
  }
  
  // Otherwise creates default DSP keeping the same input/output number
  fDSPfactory = createDSPFactoryFromString("default", DEFAULT_CODE, 0, 0, getTarget(), error, LLVM_OPTIMIZATION);
  dsp = createDSPInstance(fDSPfactory);
  logStr << "Allocation of default DSP succeeded, " <<  dsp->getNumInputs() << " input(s), " << dsp->getNumOutputs() << " output(s)";
  
end:
  LOG(logStr);
  jassert(dsp);

  // Prepare JSON
  JSONUI builder(dsp->getNumInputs(), dsp->getNumOutputs());
  metadataDSPFactory(fDSPfactory, &builder);
  dsp->buildUserInterface(&builder);
  fJSON = String(builder.JSON());
  
  LOG(fJSON);
  return dsp;
}

void faustgen_factory::add_library_path(const String& library_path)
{
  if ((library_path != "") && find(fLibraryPath.begin(), fLibraryPath.end(), library_path) == fLibraryPath.end())
  {
    fLibraryPath.push_back(library_path);
  }
}

void faustgen_factory::add_compile_option(const String& key, const String& value)
{
  if ((value != "") && find(fCompileOptions.begin(), fCompileOptions.end(), value) == fCompileOptions.end())
  {
    fCompileOptions.push_back(key);
    fCompileOptions.push_back(value);
  }
}

void faustgen_factory::add_compile_option(const String& value)
{
  if ((value != "") && find(fCompileOptions.begin(), fCompileOptions.end(), value) == fCompileOptions.end())
  {
    fCompileOptions.push_back(value);
  }
}

void faustgen_factory::print_compile_options()
{
  if (fCompileOptions.size() > 0)
  {
    LOG("-----------------------------");
    StringVectorIt it;
    for (it = fCompileOptions.begin(); it != fCompileOptions.end(); it++)
    {
      LOG("Compile option =" + *it);
    }
    LOG("-----------------------------");
  }
}

void faustgen_factory::default_compile_options()
{
  // Clear and set default value
  fCompileOptions.clear();
  
  // By default when double
  if (sizeof(FAUSTFLOAT) == 8)
  {
    add_compile_option("-double");
  }
  
  // Add -svg to current compile options
  add_compile_option("-svg");
  
  // All library paths
  StringVectorIt it;
  for (it = fLibraryPath.begin(); it != fLibraryPath.end(); it++)
  {
    add_compile_option("-I", *it);
  }
  
  // Draw path
  add_compile_option("-O", fDrawPath);
  
  // All options set in the 'compileoptions' message
  for (it = fOptions.begin(); it != fOptions.end(); it++)
  {
    add_compile_option(*it);
  }
  
  // Vector mode by default
  /*
   add_compile_option("-vec");
   add_compile_option("-lv");
   add_compile_option("1");
   */
  /*
   Seems not necessary...
   fCompileOptions.push_back("-vs");
   stringstream num;
   num << sys_getblksize();
   add_compile_option(num.str());
   */
}

void faustgen_factory::getStateInformation (XmlElement& xml)
{
  xml.setAttribute ("version", FAUSTGEN_VERSION);
  
  if (fSourceCode.length())
  {
    xml.setAttribute ("sourcecode", fSourceCode);
  }
  
  if (fDSPfactory)
  {
    string bitcode = writeDSPFactoryToBitcode(fDSPfactory);
    xml.setAttribute ("bitcode", bitcode);
  }
}

void faustgen_factory::setStateInformation (XmlElement& xml)
{
  String faustgen_version = xml.getStringAttribute("version");
  String sourcecode;
  String bitcode;
  
  if (faustgen_version == String::empty)
  {
    LOG("Cannot read \"version\" key, so ignore bitcode, force recompilation and use default compileoptions");
    goto default_sourcecode;
  }
  else if (faustgen_version != FAUSTGEN_VERSION)
  {
    String logStr;
    logStr << "Older version of faustgen (" << FAUSTGEN_VERSION << "versus " << faustgen_version << "), so ignore bitcode, force recompilation and use default compileoptions";
    LOG(logStr);
    goto read_sourcecode;
  }
  
  bitcode = xml.getStringAttribute("bitcode");

  if (bitcode != String::empty)
  {
    fBitCode = bitcode;
  }
  
read_sourcecode:
  sourcecode = xml.getStringAttribute("sourcecode");
  
  if (sourcecode != String::empty)
  {
    fSourceCode = sourcecode;
    return;
  }
  
default_sourcecode:
  fSourceCode = DEFAULT_CODE;
}

bool faustgen_factory::try_open_svg()
{
  // Open the svg diagram file inside a web browser
  char command[512];
#ifdef WIN32
  sprintf(command, "type \"file:///%sfaustgen-%d-svg/process.svg\"", fDrawPath.toRawUTF8(), fFaustNumber);
#else
  sprintf(command, "open -a Safari \"file://%sfaustgen-%d-svg/process.svg\"", fDrawPath.toRawUTF8(), fFaustNumber);
#endif
  return (system(command) == 0);
}

void faustgen_factory::open_svg()
{
  // Open the svg diagram file inside a web browser
  char command[512];
#ifdef WIN32
  sprintf(command, "start \"\" \"file:///%sfaustgen-%d-svg/process.svg\"", fDrawPath.toRawUTF8(), fFaustNumber);
#else
  sprintf(command, "open -a Safari \"file://%sfaustgen-%d-svg/process.svg\"", fDrawPath.toRawUTF8(), fFaustNumber);
#endif
  //LOG("open_svg %s", command);
  system(command);
}

void faustgen_factory::remove_svg()
{
  // Possibly done by "compileoptions" or display_svg
  char command[512];
#ifdef WIN32
  sprintf(command, "rmdir /S/Q \"%sfaustgen-%d-svg\"", fDrawPath.toRawUTF8(), fFaustNumber);
#else
  sprintf(command, "rm -r \"%sfaustgen-%d-svg\"", fDrawPath.toRawUTF8(), fFaustNumber);
#endif
  system(command);
}

void faustgen_factory::display_svg()
{
  // Try to open SVG svg diagram file inside a web browser
  if (!try_open_svg())
  {
    LOG("SVG diagram not available, recompile to produce it");
    
    // Force recompilation to produce it
    llvm_dsp_factory* factory = create_factory_from_sourcecode(0);
    //deleteDSPFactory(factory); // commented out in faustgen~
    
    // Open the SVG diagram file inside a web browser
    open_svg();
  }
}

String faustgen_factory::get_svg_path()
{
  String path;
  path << "file://" << fDrawPath << "faustgen-" << fFaustNumber << "-svg/process.svg";
  return path;
}

void faustgen_factory::update_sourcecode(String source_code, FaustAudioProcessor* instance)
{
  // Recompile only if text has been changed
  if (fSourceCode != source_code)
  {
    
    // Update all instances
    set<FaustAudioProcessor*>::const_iterator it;
    for (it = fInstances.begin(); it != fInstances.end(); it++)
    {
      (*it)->hilight_off();
    }
    
    // Delete the existing Faust module
    free_dsp_factory();

    fSourceCode = source_code;
    
    // Update all instances
    fUpdateInstance = instance;
    for (it = fInstances.begin(); it != fInstances.end(); it++)
    {
      (*it)->update_sourcecode();
    }
    
  }
  else
  {
    LOG("DSP code has not been changed...");
  }
}

//void faustgen_factory::read(File path)
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
//      LOG("Faust DSP file not found");
//      return;
//    }
//    // Otherwise locate the file
//  } else {
//    strcpy(filename, s->s_name);
//    if (locatefile_extended(filename, &path, (t_fourcc*)&type, (t_fourcc*)&type, 1)) {
//      LOG("Faust DSP file '%s' not found", filename);
//      return;
//    }
//  }
//  
//  // File found, open it and recompile DSP
//  err = path_opensysfile(filename, path, &fh, READ_PERM);
//  if (err) {
//    LOG("Faust DSP file '%s' cannot be opened", filename);
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
//    LOG("Faust DSP file '%s' cannot be read", filename);
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

//void faustgen_factory::write(File path)
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
//      LOG("Faust DSP file not found");
//      return;
//    } else {
//      err = path_createsysfile(filename, path, type, &fh);
//      if (err) {
//        LOG("Faust DSP file '%s' cannot be created", filename);
//        return;
//      }
//    }
//    // Otherwise locate or create the file
//  } else {
//    strcpy(filename, s->s_name);
//    if (locatefile_extended(filename, &path, (t_fourcc*)&type, (t_fourcc*)&type, 1)) {
//      LOG("Faust DSP file '%s' not found, so tries to create it", filename);
//      err = path_createsysfile(filename, path, type, &fh);
//      if (err) {
//        LOG("Faust DSP file '%s' cannot be created", filename);
//        return;
//      }
//    } else {
//      err = path_opensysfile(filename, path, &fh, WRITE_PERM);
//      if (err) {
//        LOG("Faust DSP file '%s' cannot be opened", filename);
//        return;
//      }
//    }
//  }
//  
//  err = sysfile_writetextfile(fh, fSourceCode, (t_sysfile_text_flags)(TEXT_LB_UNIX | TEXT_NULL_TERMINATE));
//  if (err) {
//    LOG("Faust DSP file '%s' cannot be written", filename);
//  }
//  sysfile_close(fh);
//}
