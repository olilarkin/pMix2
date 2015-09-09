/*
  ==============================================================================

    FaustGenFactory.cpp
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

#include "FaustGenFactory.h"
#include "FaustAudioPluginInstance.h"
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
static std::string getTarget() { return ""; }
#endif

FaustgenFactory::FaustgenFactory(const String& name, const File& path, const File& svgPath)
{
  fUpdateInstance = 0;
  fName = name;
  fDSPfactory = 0;
  fSourceCode = DEFAULT_CODE;
  gFaustCounter++;
  fFaustNumber = gFaustCounter;
  
  // Built the complete resource path
  fLibraryPath.add(path);
  fDrawPath = svgPath;
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
  std::set<FaustAudioPluginInstance*>::const_iterator it;
  for (it = fInstances.begin(); it != fInstances.end(); it++) {
    (*it)->freeDSP();
  }

  deleteDSPFactory(fDSPfactory);
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

llvm_dsp_factory* FaustgenFactory::createFactoryFromSourceCode(FaustAudioPluginInstance* instance)
{
  String name_app;
  name_app << "faustgen-" << fFaustNumber;
  
  // To be sure we get a correct SVG diagram...
  removeSVG();
  
  defaultCompileOptions();
  //printCompileOptions();
  
  // Prepare compile options
  std::string error;
  const char* argv[32];
  memset(argv, 0, 32 * sizeof(char*));
  
  jassert(fCompileOptions.size() < 32);
  
  for (int opt = 0; opt < fCompileOptions.size(); opt++)
  {
    argv[opt] = (char*) fCompileOptions.getReference(opt).toRawUTF8();
  }
  
  if (fDrawPath != File::nonexistent)
  {
    // Generate SVG file
    if (!generateAuxFilesFromString(name_app.toStdString(), fSourceCode.toStdString(), fCompileOptions.size(), argv, error))
    {
      //TODO: if there is an error here STOP
      LOG("Generate SVG error : " + error);
    }

    File svgFile = getSVGFile();
    
    if (svgFile.exists())
    {
      File htmlFile(fDrawPath.getFullPathName() + "/" + getSVGFolderName() + "/index.html");
      htmlFile.appendText(HTML_WRAPPER);
    }
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
    LOG("Invalid Faust code or compile options:" + error);
    return 0;
  }
}

llvm_dsp* FaustgenFactory::createDSPAux(FaustAudioPluginInstance* instance)
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

  if (fDSPfactory)
  {
    dsp = createDSPInstance(fDSPfactory);
    logStr << "Allocation of default DSP succeeded, " << dsp->getNumInputs() << " input(s), " << dsp->getNumOutputs() << " output(s)";
  }

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
  if ((value != String::empty) && !fCompileOptions.contains(value))
  {
    fCompileOptions.add(key);
    fCompileOptions.add(value);
  }
}

void FaustgenFactory::addCompileOption(const String& value)
{
  if ((value != String::empty))
  {
    fCompileOptions.addIfNotAlreadyThere(value);
  }
}

void FaustgenFactory::printCompileOptions()
{
  if (fCompileOptions.size() > 0)
  {
    LOG("-----------------------------");
    for (int opt = 0; opt < fCompileOptions.size(); opt++)
    {
      LOG("Compile option =" + fCompileOptions.getReference(opt));
    }
    LOG("-----------------------------");
  }
}

void FaustgenFactory::defaultCompileOptions()
{
  fCompileOptions.clear();
  
  // By default when double
  if (sizeof(FAUSTFLOAT) == 8)
    addCompileOption("-double");
  
  if (fDrawPath != File::nonexistent)
    addCompileOption("-svg");
  
  for (int path=0;path<fLibraryPath.getNumPaths();path++)
    addCompileOption("-I", fLibraryPath[path].getFullPathName());
  
  // Draw path
  if (fDrawPath != File::nonexistent)
    addCompileOption("-O", fDrawPath.getFullPathName());
  
  //addCompileOption("-o", "tmp1.cpp");
  
  for (int opt = 0; opt < fExtraOptions.size(); opt++)
  {
    addCompileOption(fExtraOptions.getReference(opt));
  }
  
  // Vector mode by default
  /*
   addCompileOption("-vec");
   addCompileOption("-lv");
   addCompileOption("1");
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

void FaustgenFactory::removeSVG()
{
  if (fDrawPath != File::nonexistent)
  {
    getSVGFile().getParentDirectory().deleteRecursively();
  }
}

void FaustgenFactory::displaySVG()
{
//  // Try to open SVG svg diagram file inside a web browser
//  if (!tryOpenSVG())
//  {
//    LOG("SVG diagram not available, recompile to produce it");
//    
//    // Force recompilation to produce it
//    llvm_dsp_factory* factory = createFactoryFromSourceCode(0);
//    //deleteDSPFactory(factory); // commented out in faustgen~
//    
//    // Open the SVG diagram file inside a web browser
//    openSVG();
//  }
}

File FaustgenFactory::getSVGFile()
{
  File svgPathForThisInstance(fDrawPath.getChildFile(getSVGFolderName()));
  return svgPathForThisInstance.getChildFile("process.svg");
}

String FaustgenFactory::getHTMLURI()
{
  File svgPathForThisInstance(fDrawPath.getChildFile(getSVGFolderName()));
    
  String URI;
  URI << "file://" << svgPathForThisInstance.getChildFile("index.html").getFullPathName(); //TODO: will this work on windows?
  return URI;
}

String FaustgenFactory::getSVGFolderName()
{
  String name;
  name << "faustgen-" << fFaustNumber << "-svg";
  return name;
}

void FaustgenFactory::updateSourceCode(String sourceCode, FaustAudioPluginInstance* instance)
{
  // Recompile only if text has been changed
  if (fSourceCode != sourceCode)
  {
    
    // Update all instances
    std::set<FaustAudioPluginInstance*>::const_iterator it;
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
      (*it)->createDSP();
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
