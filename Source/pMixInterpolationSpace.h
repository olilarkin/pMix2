/*
  ==============================================================================

    pMixInterpolationSpace.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#pragma once

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
  std::unique_ptr<PMixInterpolationSpaceLayout> layout;
  std::unique_ptr<pMixInterpolationSpaceCrossHairs> crosshairs;
};

