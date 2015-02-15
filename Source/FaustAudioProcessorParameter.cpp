/*
  ==============================================================================

    FaustAudioProcessorParameter.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "FaustAudioProcessorParameter.h"

//FaustAudioProcessorParameter::FaustAudioProcessorParameter(var uielement)
//: name(uielement["label"].toString())
//, value(uielement["init"])
//, defaultValue(uielement["init"])
//, minValue(uielement["min"])
//, maxValue(uielement["max"])
//, step(uielement["step"])
//, address(uielement["address"].toString())
//{
//  if (uielement["type"] == "button")
//  {
//  }
//  else if (uielement["type"] == "nentry") // could be text box or enum
//  {
//    if (uielement["meta"])
//    {
//      if (uielement["meta"][1]["tooltip"])
//      {
//        //TODO: improve horrible parsing of tooltip string
//        String tt = uielement["meta"][1]["tooltip"].toString();
//        
//        if (tt.contains(";"))
//        {
//          while(tt.length())
//          {
//            String subStr = tt.upToFirstOccurrenceOf(";", true, false);
//            nEntryLabels.add(subStr);
//            tt = tt.trimCharactersAtStart(subStr);
//            tt = tt.trimStart();
//          }
//        }
//        else
//          tooltip = uielement["meta"][1]["tooltip"].toString();
//      }
//    }
//  }
//  else
//  {
//    if (uielement["meta"])
//    {
//      if (uielement["meta"].isArray())
//      {
//        if (uielement["meta"].size() > 2)
//        {
//          if (uielement["meta"][1]["tooltip"])
//            tooltip = uielement["meta"][1]["tooltip"].toString();
//        }
//      }
//    }
//  }
//  
//  if (uielement["meta"]["units"])
//    unitsLabel = uielement["meta"]["units"].toString();
//}

FaustAudioProcessorParameter::FaustAudioProcessorParameter(String name, EParamType type, float defaultVal, float minVal, float maxVal, float step, String units, FAUSTFLOAT* zone)
: name(name)
, type(type)
, valueNormalized(toNormalizedValue(defaultVal, minVal, maxVal))
, defaultValueNormalized(toNormalizedValue(defaultVal, minVal, maxVal))
, minValue(minVal)
, maxValue(maxVal)
, step(step)
, zone(zone)
{
}

void FaustAudioProcessorParameter::setValue(float newValue)
{
  valueNormalized = newValue;
  *zone = fromNormalizedValue(valueNormalized, minValue, maxValue);
}

float FaustAudioProcessorParameter::getValueForText(const String& text) const
{
  return 0.;
}

String FaustAudioProcessorParameter::getText (float value, int maximumStringLength) const
{
  String val;
  val << fromNormalizedValue(value, minValue, maxValue);
  return val;
}
