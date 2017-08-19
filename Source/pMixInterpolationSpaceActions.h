/*
  ==============================================================================

    pMixInterpolationSpaceActions.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class PMixInterpolationSpaceLayout;

class MovePresetAction : public UndoableAction
{
public:
  MovePresetAction (PMixInterpolationSpaceLayout* interpolationSpace, const String& componentID, Rectangle<int> startBounds, Rectangle<int> endBounds) noexcept;
  bool perform() override;
  bool undo() override;
  int getSizeInUnits() override;
  
  void doStuff(Rectangle<int>& whichBounds);
  
private:
  PMixInterpolationSpaceLayout* interpolationSpace;
  String componentID;
  Rectangle<int> startBounds;
  Rectangle<int> endBounds;
  JUCE_DECLARE_NON_COPYABLE (MovePresetAction)
};

