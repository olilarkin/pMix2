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
#include "pMixInterpolationSpaceLayout.h"

class InterpolationSpaceIPos : public Component
{
public:
  InterpolationSpaceIPos(PMixAudioEngine& audioEngine, PMixInterpolationSpaceLayout& layout, const uint32 nodeId, Colour colour);
  ~InterpolationSpaceIPos();
  
  void resized () override;
  void mouseDown (const MouseEvent& e) override;
  void mouseDrag (const MouseEvent& e) override;
  void mouseUp (const MouseEvent& e) override;
  void paint (Graphics& g) override;

  void update();

private:
  PMixAudioEngine& audioEngine;
  PMixInterpolationSpaceLayout& layout;

  ComponentDragger myDragger;
  ComponentBoundsConstrainer boundsConstrainer;
  Colour colour;
  
public:
  const uint32 nodeId;
};

class pMixInterpolationSpaceCrossHairs : public Component
                                       , public ChangeListener
{
public:
  pMixInterpolationSpaceCrossHairs(PMixAudioEngine& audioEngine, PMixInterpolationSpaceLayout& layout);
  ~pMixInterpolationSpaceCrossHairs();
  
  void paint (Graphics&) override;
  void resized() override;
  
  void changeListenerCallback (ChangeBroadcaster* source) override;
  
  void updateComponents();
  InterpolationSpaceIPos* getComponentForNode (const uint32 nodeId) const;

private:
  PMixAudioEngine& audioEngine;
  PMixInterpolationSpaceLayout& layout;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pMixInterpolationSpaceCrossHairs)
};



#endif  // PMIXINTERPOLATIONSPACECROSSHAIRS_H_INCLUDED
