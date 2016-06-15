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
  doStuff(endBounds);
  return true;
}

bool MovePresetAction::undo()
{
  doStuff(startBounds);
  return true;
}

int MovePresetAction::getSizeInUnits()
{
  return (int) sizeof (*this); //xxx should be more accurate
}

void MovePresetAction::doStuff(Rectangle<int>& whichBounds)
{
  InterpolationSpacePreset* presetComp = dynamic_cast<InterpolationSpacePreset*>(interpolationSpace->findChildWithID(componentID));
  presetComp->setBounds(whichBounds);
  
  Point<double> normalizedPos;
  normalizedPos.x = whichBounds.getCentreX() / (double) presetComp->getParentWidth();
  normalizedPos.y = whichBounds.getCentreY() / (double) presetComp->getParentHeight();
  presetComp->audioEngine.getDoc().setPresetPosition(presetComp->nodeId, presetComp->presetId, normalizedPos.x, normalizedPos.y);
  
  interpolationSpace->repaintPresetsForNode(presetComp->nodeId);
}