#include "pMixInterpolationSpace.h"

InterpolationSpace::InterpolationSpace (PMixAudioEngine& audioEngine)
{
  addAndMakeVisible(layout = new PMixInterpolationSpaceLayout(audioEngine));
}

InterpolationSpace::~InterpolationSpace ()
{
}

void InterpolationSpace::resized ()
{
  layout->setBounds(getLocalBounds());
}

void InterpolationSpace::paint (Graphics& g)
{
}