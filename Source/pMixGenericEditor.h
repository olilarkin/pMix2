/*
  ==============================================================================

    pMixGenericEditor.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#ifndef PMIXGENERICEDITOR_H_INCLUDED
#define PMIXGENERICEDITOR_H_INCLUDED

#include "JuceHeader.h"

class PMixGenericAudioProcessorEditor : public AudioProcessorEditor
{
public:
  PMixGenericAudioProcessorEditor (AudioProcessor* owner);
  ~PMixGenericAudioProcessorEditor();
  
  void paint (Graphics&) override;
  void resized() override;
  
private:
  PropertyPanel panel;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PMixGenericAudioProcessorEditor)
};


#endif  // PMIXGENERICEDITOR_H_INCLUDED
