/*
  ==============================================================================

    pMixGraphEditorActions.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixGraphEditorActions.h"
#include "pMixPluginWindow.h"

CreateFilterAction::CreateFilterAction (PMixAudioEngine& audioEngine, const PluginDescription* desc, double x, double y) noexcept
: audioEngine(audioEngine)
, x(x)
, y(y)
, desc(desc)
{
}

bool CreateFilterAction::perform()
{
  nodeID = audioEngine.getDoc().addFilter (desc, x, y);
  
  if (nodeID < 0xFFFFFFFF)
    return true;
  else
    return false;
  
}

bool CreateFilterAction::undo()
{
  audioEngine.getDoc().removeFilter(nodeID);
  
  return true;
}

int CreateFilterAction::getSizeInUnits()
{
  return (int) sizeof (*this); //xxx should be more accurate
}

RemoveFilterAction::RemoveFilterAction (PMixAudioEngine& audioEngine, PluginDescription desc, uint32 nodeID, double x, double y) noexcept
: audioEngine(audioEngine)
, x(x)
, y(y)
, desc(desc)
, nodeID(nodeID)
{
}

bool RemoveFilterAction::perform()
{
  PluginWindow::closeCurrentlyOpenWindowsFor (nodeID);
  audioEngine.getDoc().removeFilter (nodeID);
  
  if (nodeID < 0xFFFFFFFF)
    return true;
  else
    return false;
  
}

bool RemoveFilterAction::undo()
{
  audioEngine.getDoc().addFilter(&desc, x, y);
  
  return true;
}

int RemoveFilterAction::getSizeInUnits()
{
  return (int) sizeof (*this); //xxx should be more accurate
}

MoveFilterAction::MoveFilterAction (PMixAudioEngine& audioEngine, FilterComponent* filterComponent, uint32 nodeID, Point<double> startPos, Point<double> endPos) noexcept
: audioEngine(audioEngine)
, filterComponent(filterComponent)
, nodeID(nodeID)
, startPos(startPos)
, endPos(endPos)
{
}

bool MoveFilterAction::perform()
{
  audioEngine.getDoc().setNodePosition (nodeID, endPos.x, endPos.y);
  filterComponent->getGraphPanel()->updateComponents();
  return true;
}

bool MoveFilterAction::undo()
{
  audioEngine.getDoc().setNodePosition (nodeID, startPos.x, startPos.y);
  filterComponent->getGraphPanel()->updateComponents();
  return true;
}

int MoveFilterAction::getSizeInUnits()
{
  return (int) sizeof (*this); //xxx should be more accurate
}