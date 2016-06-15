/*
  ==============================================================================

    pMixInterpolationSpace.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#ifndef PMIXINTERPOLATIONSPACE_H_INCLUDED
#define PMIXINTERPOLATIONSPACE_H_INCLUDED

#include "JuceHeader.h"
#include "pMixGraphEditor.h"
#include "pMixAudioEngine.h"
#include "pMixInterpolationSpaceLayout.h"
#include "pMixInterpolationSpaceCrosshairs.h"

class InterpolationSpace : public Component
{
public:
  InterpolationSpace (PMixAudioEngine& audioEngine, GraphEditor& graphEditor);
  ~InterpolationSpace ();
  void resized () override;
  void paint (Graphics& g) override;
  
private:
  ScopedPointer<PMixInterpolationSpaceLayout> layout;
  ScopedPointer<pMixInterpolationSpaceCrossHairs> crosshairs;
};

#endif //PMIXINTERPOLATIONSPACE_H_INCLUDED 