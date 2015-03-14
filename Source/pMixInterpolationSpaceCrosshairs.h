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

class InterpolationSpaceIPos : public Component
{
public:
  InterpolationSpaceIPos(PMixAudioEngine& audioEngine, const uint32 filterID, Colour colour);
  ~InterpolationSpaceIPos();
  
  void resized ();
  void mouseDown (const MouseEvent& e);
  void mouseDrag (const MouseEvent& e);
  void mouseUp (const MouseEvent& e);
  void paint (Graphics& g);
  
  void addPresetLocation(float x, float y);
  
private:
  PMixAudioEngine& audioEngine;
  const uint32 filterID;
  ComponentDragger myDragger;
  ComponentBoundsConstrainer boundsConstrainer;
  Colour colour;
};

class pMixInterpolationSpaceCrossHairs : public Component
                                       , public ChangeListener
{
public:
  pMixInterpolationSpaceCrossHairs(PMixAudioEngine& audioEngine);
  ~pMixInterpolationSpaceCrossHairs();
  
  void paint (Graphics&);
  void resized();
  
  void changeListenerCallback (ChangeBroadcaster* source);
  
  void updateComponents();

private:
  PMixAudioEngine& audioEngine;
  Array<Array<Point<float>>> presetsLocations;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pMixInterpolationSpaceCrossHairs)
};



#endif  // PMIXINTERPOLATIONSPACECROSSHAIRS_H_INCLUDED
