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

RemoveFilterAction::RemoveFilterAction (PMixAudioEngine& audioEngine, uint32 nodeID) noexcept
: audioEngine(audioEngine)
, nodeID(nodeID)
{
  nodeXML = audioEngine.getDoc().createNodeXml(audioEngine.getGraph().getNodeForId(nodeID));
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
  audioEngine.getDoc().createNodeFromXml(*nodeXML);
  audioEngine.getDoc().changed();
  
  return true;
}

int RemoveFilterAction::getSizeInUnits()
{
  return (int) sizeof (*this); //xxx should be more accurate
}

MoveFilterAction::MoveFilterAction (PMixAudioEngine& audioEngine, GraphEditor& graphEditor, uint32 nodeID, Point<double> startPos, Point<double> endPos) noexcept
: audioEngine(audioEngine)
, graphEditor(graphEditor)
, nodeID(nodeID)
, startPos(startPos)
, endPos(endPos)
{
}

bool MoveFilterAction::perform()
{
  audioEngine.getDoc().setNodePosition (nodeID, endPos.x, endPos.y);
  graphEditor.updateComponents();
  return true;
}

bool MoveFilterAction::undo()
{
  audioEngine.getDoc().setNodePosition (nodeID, startPos.x, startPos.y);
  graphEditor.updateComponents();
  return true;
}

int MoveFilterAction::getSizeInUnits()
{
  return (int) sizeof (*this); //xxx should be more accurate
}