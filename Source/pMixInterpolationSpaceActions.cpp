/*
  ==============================================================================

    pMixInterpolationSpaceActions.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixInterpolationSpaceActions.h"

MovePresetAction::MovePresetAction (Component* interpolationSpace, const String& componentID, Rectangle<int> startBounds, Rectangle<int> endBounds) noexcept
: interpolationSpace(interpolationSpace)
, componentID(componentID)
, startBounds(startBounds)
, endBounds(endBounds)
{
}

bool MovePresetAction::perform()
{
  interpolationSpace->findChildWithID(componentID)->setBounds(endBounds);
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