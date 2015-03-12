/*
  ==============================================================================

    pMixInterpolationSpaceCrosshairs.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixInterpolationSpaceCrosshairs.h"

pMixInterpolationSpaceCrossHairs::pMixInterpolationSpaceCrossHairs(PMixAudioEngine& audioEngine)
: audioEngine(audioEngine)
{
  
}

pMixInterpolationSpaceCrossHairs::~pMixInterpolationSpaceCrossHairs()
{
  
}

void pMixInterpolationSpaceCrossHairs::changeListenerCallback (ChangeBroadcaster* source)
{
}

void pMixInterpolationSpaceCrossHairs::paint (Graphics& g)
{
  g.fillAll (Colours::white);   // clear the background
  
  g.setColour (Colours::grey);
  g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
  
  g.setColour (Colours::lightblue);
  g.setFont (14.0f);
  g.drawText ("pMixInterpolationSpaceLayout", getLocalBounds(),
              Justification::centred, true);   // draw some placeholder text
}

void pMixInterpolationSpaceCrossHairs::resized()
{
}
