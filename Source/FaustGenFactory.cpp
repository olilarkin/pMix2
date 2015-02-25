/*
  ==============================================================================

    FaustGenFactory.cpp
    Based on Faustgen~ by GRAME
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "FaustGenFactory.h"
#include "FaustAudioProcessor.h"
#include <stdio.h>

int FaustgenFactory::gFaustCounter = 0;
std::map<String, FaustgenFactory*> FaustgenFactory::gFactoryMap;

#if JUCE_MAC
static std::string getTarget()
{
  int tmp;
  return (sizeof(&tmp) == 8) ? "" : "i386-apple-darwin10.6.0";
}
#else
//#elif JUCE_WIN32
static string getTarget() { return ""; }
#endif

FaustgenFactory::FaustgenFactory(const String& name, const File& path)
{
  fUpdateInstance = 0;
  fName = name;
  fDSPfactory = 0;
  fSourceCode = DEFAULT_CODE;
  gFaustCounter++;
  fFaustNumber = gFaustCounter;
  
  // Built the complete resource path
  fLibraryPath.add(path);
  
#if JUCE_MAC
  // Draw path in temporary folder
  fDrawPath = std::string(FAUST_DRAW_PATH);
#endif
}

FaustgenFactory::~FaustgenFactory()
{
  freeDSPFactory();
  removeSVG();
}

void FaustgenFactory::freeDSPFactory()
{
  const ScopedLock lock(fDSPMutex);

  // Free all instances
  std::set<FaustAudioProcessor*>::const_iterator it;
  for (it = fInstances.begin(); it != fInstances.end(); it++) {
    (*it)->freeDSP();
  }

  //deleteDSPFactory(fDSPfactory); //commented out in faustgen~
  fDSPfactory = 0;
}

llvm_dsp_factory* FaustgenFactory::createFactoryFromBitcode()
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

llvm_dsp_factory* FaustgenFactory::createFactoryFromSourceCode(FaustAudioProcessor* instance)
{
  String name_app;
  name_app << "faustgen-" << fFaustNumber;
  
  // To be sure we get a correct SVG diagram...
  removeSVG();
  
  defaultCompileOptions();
  //printCompileOptions();
  
  // Prepare compile options
  std::string error;
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
    //TODO: if there is an error here STOP
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
      instance->highlightON(error);
    }
    LOG("Invalid Faust code or compile options : %s" + error);
    return 0;
  }
}

llvm_dsp* FaustgenFactory::createDSPAux(FaustAudioProcessor* instance)
{
  llvm_dsp* dsp = 0;
  std::string error;
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
    fDSPfactory = createFactoryFromBitcode();
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
    fDSPfactory = createFactoryFromSourceCode(instance);
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
  
  //LOG(fJSON);
  return dsp;
}

void FaustgenFactory::addLibraryPath(const File& libraryPath)
{
  fLibraryPath.addIfNotAlreadyThere(libraryPath);
}

void FaustgenFactory::addCompileOption(const String& key, const String& value)
{
  if ((value != String::empty) && find(fCompileOptions.begin(), fCompileOptions.end(), value) == fCompileOptions.end())
  {
    fCompileOptions.push_back(key);
    fCompileOptions.push_back(value);
  }
}

void FaustgenFactory::addCompileOption(const String& value)
{
  if ((value != String::empty) && find(fCompileOptions.begin(), fCompileOptions.end(), value) == fCompileOptions.end())
  {
    fCompileOptions.push_back(value);
  }
}

void FaustgenFactory::printCompileOptions()
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

void FaustgenFactory::defaultCompileOptions()
{
  // Clear and set default value
  fCompileOptions.clear();
  
  // By default when double
  if (sizeof(FAUSTFLOAT) == 8)
  {
    addCompileOption("-double");
  }
  
  // Add -svg to current compile options
  addCompileOption("-svg");
  
  // All library paths
  for (int path=0;path<fLibraryPath.getNumPaths();path++)
  {
    addCompileOption("-I", fLibraryPath[path].getFullPathName());
  }
  
  // Draw path
  addCompileOption("-O", fDrawPath);
  addCompileOption("-o", "tmp1.cpp");
  
  StringVectorIt it;
  // All options set in the 'compileoptions' message
  for (it = fOptions.begin(); it != fOptions.end(); it++)
  {
    addCompileOption(*it);
  }
  
  // Vector mode by default
  /*
   addCompileOption("-vec");
   addCompileOption("-lv");
   addCompileOption("1");
   */
  /*
   Seems not necessary...
   fCompileOptions.push_back("-vs");
   stringstream num;
   num << sys_getblksize();
   addCompileOption(num.str());
   */
}

void FaustgenFactory::getStateInformation (XmlElement& xml)
{
  xml.setAttribute ("version", FAUSTGEN_VERSION);
  
  if (fSourceCode.length())
  {
    xml.setAttribute ("sourcecode", fSourceCode);
  }
  
  if (fDSPfactory)
  {
    std::string bitcode = writeDSPFactoryToBitcode(fDSPfactory);
    xml.setAttribute ("bitcode", String(bitcode));
  }
}

void FaustgenFactory::setStateInformation (XmlElement& xml)
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

bool FaustgenFactory::tryOpenSVG()
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

void FaustgenFactory::openSVG()
{
  // Open the svg diagram file inside a web browser
  char command[512];
#ifdef WIN32
  sprintf(command, "start \"\" \"file:///%sfaustgen-%d-svg/process.svg\"", fDrawPath.toRawUTF8(), fFaustNumber);
#else
  sprintf(command, "open -a Safari \"file://%sfaustgen-%d-svg/process.svg\"", fDrawPath.toRawUTF8(), fFaustNumber);
#endif
  //LOG("openSVG %s", command);
  system(command);
}

void FaustgenFactory::removeSVG()
{
  // Possibly done by "compileoptions" or displaySVG
  char command[512];
#ifdef WIN32
  sprintf(command, "rmdir /S/Q \"%sfaustgen-%d-svg\"", fDrawPath.toRawUTF8(), fFaustNumber);
#else
  sprintf(command, "rm -r \"%sfaustgen-%d-svg\"", fDrawPath.toRawUTF8(), fFaustNumber);
#endif
  system(command);
}

void FaustgenFactory::displaySVG()
{
  // Try to open SVG svg diagram file inside a web browser
  if (!tryOpenSVG())
  {
    LOG("SVG diagram not available, recompile to produce it");
    
    // Force recompilation to produce it
    llvm_dsp_factory* factory = createFactoryFromSourceCode(0);
    //deleteDSPFactory(factory); // commented out in faustgen~
    
    // Open the SVG diagram file inside a web browser
    openSVG();
  }
}

String FaustgenFactory::getSVGPath()
{
  String path;
  path << "file://" << fDrawPath << "faustgen-" << fFaustNumber << "-svg/process.svg";
  return path;
}

void FaustgenFactory::updateSourceCode(String sourceCode, FaustAudioProcessor* instance)
{
  // Recompile only if text has been changed
  if (fSourceCode != sourceCode)
  {
    
    // Update all instances
    std::set<FaustAudioProcessor*>::const_iterator it;
    for (it = fInstances.begin(); it != fInstances.end(); it++)
    {
      (*it)->highlightOFF();
    }
    
    // Delete the existing Faust module
    freeDSPFactory();

    fSourceCode = sourceCode;
    
    // Update all instances
    fUpdateInstance = instance;
    for (it = fInstances.begin(); it != fInstances.end(); it++)
    {
      (*it)->updateSourcecode();
    }
    
  }
  else
  {
    LOG("DSP code has not been changed...");
  }
}

//void FaustgenFactory::read(File path)
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
//  freeDSPFactory();
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
//    addLibraryPath(getFolderFromFilename(full_path));
//  }
//  
//  // Update all instances
//  set<faustgen*>::const_iterator it;
//  for (it = fInstances.begin(); it != fInstances.end(); it++) {
//    (*it)->updateSourcecode();
//  }
//}

//void FaustgenFactory::write(File path)
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
