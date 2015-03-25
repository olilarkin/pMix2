/*
  ==============================================================================

    pMixConstants.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#ifndef PMIXCONSTANTS_H_INCLUDED
#define PMIXCONSTANTS_H_INCLUDED

#include "JuceHeader.h"


#define OFFSET_BETWEEN_PINS 15
#define PINS_LEFT_OFFSET 10
#define MIN_RADIUS 50.f
#define RADIUS_RANGE 100.f

#define MAX_NAUDIO_IO 32

#if JUCE_MAC
#define FAUST_LIBRARY_PATH "/Contents/Resources/"
#elif JUCE_WIN32
//#define FAUST_LIBRARY_PATH "\\faustgen-resources\\"
#endif

#if JUCE_MAC || JUCE_LINUX
#define FAUST_DRAW_PATH "/var/tmp/"
#elif JUCE_WIN32
#define FAUST_DRAW_PATH "\\faustgen-resources\\"
#endif

#endif  // PMIXCONSTANTS_H_INCLUDED
