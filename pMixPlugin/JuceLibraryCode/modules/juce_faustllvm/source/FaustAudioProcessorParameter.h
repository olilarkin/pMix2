/*
  ==============================================================================

    FaustAudioProcessorParameter.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#ifndef FaustAudioProcessorParameter_H_INCLUDED
#define FaustAudioProcessorParameter_H_INCLUDED

#include "FaustAudioPluginInstance.h"
#include "modules/juce_audio_processors/juce_audio_processors.h"

using namespace juce;

class FaustAudioProcessorParameter : public AudioProcessorParameter
{
public:
  enum EParamType { kTypeNone, kTypeBool, kTypeInt, kTypeEnum, kTypeFloat };
  
  static inline float toNormalizedValue(float nonNormalizedValue, float min, float max)
  {
    return (nonNormalizedValue - min) / (max - min);
  }
  
  static inline float fromNormalizedValue(float normalizedValue, float min, float max)
  {
    return min + normalizedValue * (max - min);
  }
  
  FaustAudioProcessorParameter(String name, EParamType type, float defaultVal, float minVal, float maxVal, float step, String units, FAUSTFLOAT* zone);
  
  float getValue() const override { return valueNormalized; }
  void setValue(float newValue) override;
  float getDefaultValue() const override { return defaultValueNormalized; }
  String getName(int maximumStringLength) const override { return name; }
  String getLabel() const override { return unitsLabel; }
  float getValueForText(const String& text) const override;
  int getNumSteps() const override { return int ((maxValue-minValue) / step ); }
  String getText (float value, int maximumStringLength) const override;

  //unique
  
  float getValueNonNormalized() { return fromNormalizedValue(valueNormalized, minValue, maxValue); }
  
private:
  String name; // Faust calls this "label"
  EParamType type;
  String unitsLabel; // Metadata in the faust "label" decl
  float valueNormalized;
  float defaultValueNormalized; // Faust calls this "init"
  float minValue;
  float maxValue;
  float step;
  FAUSTFLOAT* zone;
  
  String tooltip;
  //String address; // OSC style heirarchy
  StringArray nEntryLabels;
};

#endif  // FaustAudioProcessorParameter_H_INCLUDED
