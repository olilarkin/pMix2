/*
  ==============================================================================

    FaustAudioProcessorParameter.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "FaustAudioProcessorParameter.h"

FaustAudioProcessorParameter::FaustAudioProcessorParameter(var uielement)
: name(uielement["label"].toString())
, unitsLabel("") // TODO:
, value(uielement["init"])
, defaultValue(uielement["init"])
, minValue(uielement["min"])
, maxValue(uielement["max"])
, step(uielement["step"])
{
}

float FaustAudioProcessorParameter::getValueForText(const String& text) const
{
  return 0.;
}