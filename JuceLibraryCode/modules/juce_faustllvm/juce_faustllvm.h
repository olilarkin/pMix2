/*
  This file is part of the juce_faustllvm JUCE module
  Written by Oliver Larkin
  Contemporary Music Research Centre
  Music Department, University of York
  License: GPL
  
  based on Faustgen~ by GRAME
  
*/

#include <modules/juce_core/juce_core.h>
#include <modules/juce_audio_processors/juce_audio_processors.h>
#include <modules/juce_gui_basics/juce_gui_basics.h>

#define FAUSTFLOAT float

#define DEFAULT_FAUST_DSP_SEARCHPATH "~/Library/Application Support/pMix2/FaustNodes"

#define FAUSTGEN_VERSION "0.99b"

#if JUCE_MAC || JUCE_LINUX
  #define FAUST_DRAW_PATH "/var/tmp/"
  #define SEPARATOR '/'
#elif JUCE_WIN32
  #define FAUST_DRAW_PATH "\\faustgen-resources\\"
  #define SEPARATOR '\\'
#endif

#define LLVM_OPTIMIZATION 3
#define DEFAULT_CODE "declare name \"Faust Effect\";\n\ngain = hslider(\"gain\", 1., 0., 1., 0.);\nprocess = _*(gain);"
//#define DEFAULT_CODE "import(\"math.lib\"); \nimport(\"maxmsp.lib\"); \nimport(\"music.lib\"); \nimport(\"oscillator.lib\"); \nimport(\"reduce.lib\"); \nimport(\"filter.lib\"); \nimport(\"effect.lib\"); \n \nprocess=_,_;"

#define LOG Logger::getCurrentLogger()->writeToLog

#include "source/FaustAudioPluginInstance.h"
#include "source/FaustAudioProcessorParameter.h"
#include "source/FaustCodeTokenizer.h"
#include "source/FaustPluginFormat.h"
// #include "source/FaustDSPWatcher.h"
// #include "source/FaustFileFilter.h"
// #include "source/FaustGenFactory.h"
