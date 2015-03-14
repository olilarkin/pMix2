#include "pMixInterpolationSpace.h"

InterpolationSpace::InterpolationSpace (PMixAudioEngine& audioEngine)
{
  addAndMakeVisible(layout = new PMixInterpolationSpaceLayout(audioEngine));
  addAndMakeVisible(crosshairs = new pMixInterpolationSpaceCrossHairs(audioEngine));
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