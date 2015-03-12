/*
  ==============================================================================

    pMixInterpolationSpaceCrosshairs.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#ifndef PMIXINTERPOLATIONSPACECROSSHAIRS_H_INCLUDED
#define PMIXINTERPOLATIONSPACECROSSHAIRS_H_INCLUDED

#include "JuceHeader.h"
#include "pMixAudioEngine.h"

class pMixInterpolationSpaceCrossHairs : public Component
                                       , public ChangeListener
{
public:
  pMixInterpolationSpaceCrossHairs(PMixAudioEngine& audioEngine);
  ~pMixInterpolationSpaceCrossHairs();
  
  void paint (Graphics&);
  void resized();
  
  void changeListenerCallback (ChangeBroadcaster* source);
private:
  PMixAudioEngine& audioEngine;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pMixInterpolationSpaceCrossHairs)
};



#endif  // PMIXINTERPOLATIONSPACECROSSHAIRS_H_INCLUDED
