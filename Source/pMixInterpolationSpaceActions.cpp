/*
  ==============================================================================

    pMixInterpolationSpaceActions.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixInterpolationSpaceActions.h"
#include "pMixInterpolationSpaceLayout.h"

MovePresetAction::MovePresetAction (PMixInterpolationSpaceLayout* interpolationSpace, const String& componentID, Rectangle<int> startBounds, Rectangle<int> endBounds) noexcept
: interpolationSpace(interpolationSpace)
, componentID(componentID)
, startBounds(startBounds)
, endBounds(endBounds)
{
}

bool MovePresetAction::perform()
{
  InterpolationSpacePreset* presetComp = dynamic_cast<InterpolationSpacePreset*>(interpolationSpace->findChildWithID(componentID));
  presetComp->setBounds(endBounds);
  
  Point<double> normalizedPos;
  normalizedPos.x = endBounds.getX() / (double) presetComp->getParentWidth();
  normalizedPos.y = endBounds.getY() / (double) presetComp->getParentHeight();
  presetComp->audioEngine.getDoc().setPresetPosition(presetComp->nodeId, presetComp->presetIdx, normalizedPos.x, normalizedPos.y);
  return true;
}

bool MovePresetAction::undo()
{
  interpolationSpace->findChildWithID(componentID)->setBounds(startBounds);
  return true;
}

int MovePresetAction::getSizeInUnits()
{
  return (int) sizeof (*this); //xxx should be more accurate
}