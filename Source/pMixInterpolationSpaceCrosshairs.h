/*
  ==============================================================================

    pMixInterpolationSpaceCrosshairs.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "pMixAudioEngine.h"
#include "pMixInterpolationSpaceLayout.h"

class InterpolationSpaceIPos : public Component
{
public:
  InterpolationSpaceIPos(PMixAudioEngine& audioEngine, PMixInterpolationSpaceLayout& layout, NodeID nodeID, Colour colour);
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
  NodeID nodeID;
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
  InterpolationSpaceIPos* getComponentForNode (NodeID nodeID) const;

private:
  PMixAudioEngine& audioEngine;
  PMixInterpolationSpaceLayout& layout;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pMixInterpolationSpaceCrossHairs)
};



