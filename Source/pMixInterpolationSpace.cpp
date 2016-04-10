/*
  ==============================================================================

    pMixInterpolationSpace.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixInterpolationSpace.h"

InterpolationSpace::InterpolationSpace (PMixAudioEngine& audioEngine, GraphEditor& graphEditor)
{
  addAndMakeVisible(layout = new PMixInterpolationSpaceLayout(audioEngine, graphEditor));
  addAndMakeVisible(crosshairs = new pMixInterpolationSpaceCrossHairs(audioEngine, *layout));
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