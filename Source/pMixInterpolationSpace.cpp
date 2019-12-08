/*
  ==============================================================================

    pMixInterpolationSpace.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixInterpolationSpace.h"

InterpolationSpace::InterpolationSpace (PMixAudioEngine& audioEngine, GraphEditor& graphEditor)
{
  layout = std::make_unique<PMixInterpolationSpaceLayout>(audioEngine, graphEditor);
  crosshairs = std::make_unique<pMixInterpolationSpaceCrossHairs>(audioEngine, *layout);
  addAndMakeVisible(*layout);
  addAndMakeVisible(*crosshairs);
}

InterpolationSpace::~InterpolationSpace ()
{
}

void InterpolationSpace::resized ()
{
  layout->setBounds(getLocalBounds());
  crosshairs->setBounds(getLocalBounds());
}

void InterpolationSpace::paint (Graphics& g)
{
  g.fillAll (Colours::white);
}
