/*
  ==============================================================================

    FaustAudioProcessorParameter.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#ifndef FAUSTAUDIOPROCESSORPARAMETER_H_INCLUDED
#define FAUSTAUDIOPROCESSORPARAMETER_H_INCLUDED

#include "JuceHeader.h"

class FaustAudioProcessorParameter : public AudioProcessorParameter
{
public:
  FaustAudioProcessorParameter(var uielement);
  
  float getValue() const override { return value; }
  void setValue(float newValue) override { value = newValue; }
  float getDefaultValue() const override { return defaultValue; }
  String getName(int maximumStringLength) const override { return name; }
  String getLabel() const override { return unitsLabel; }
  float getValueForText(const String& text) const override;
  int getNumSteps() const override { return int ((maxValue-minValue) / step ); }

private:
  String name; // Faust calls this "label"
  String unitsLabel; // Metadata in the faust "label" decl
  float value;
  float defaultValue; // Faust calls this "init"
  float minValue;
  float maxValue;
  float step;
  
  String tooltip;
  String address; // OSC style heirarchy
  StringArray nEntryLabels;
};

#endif  // FAUSTAUDIOPROCESSORPARAMETER_H_INCLUDED
