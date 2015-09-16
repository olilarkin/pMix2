/*
  ==============================================================================

    pMixGraphEditorActions.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixGraphEditorActions.h"
#include "pMixPluginWindow.h"

CreateFilterAction::CreateFilterAction (PMixAudioEngine& audioEngine, GraphEditor& graphEditor, const PluginDescription* desc, double x, double y) noexcept
: audioEngine(audioEngine)
, graphEditor(graphEditor)
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
  graphEditor.getLassoSelection().deselectAll();

  return true;
}

int CreateFilterAction::getSizeInUnits()
{
  return (int) sizeof (*this); //xxx should be more accurate
}

RemoveFilterAction::RemoveFilterAction (PMixAudioEngine& audioEngine, GraphEditor& graphEditor, uint32 nodeID) noexcept
: audioEngine(audioEngine)
, graphEditor(graphEditor)
, nodeID(nodeID)
{
  nodeXML = audioEngine.getDoc().createNodeXml(audioEngine.getGraph().getNodeForId(nodeID));
}

bool RemoveFilterAction::perform()
{
  PluginWindow::closeCurrentlyOpenWindowsFor (nodeID);
  
  audioEngine.getDoc().removeFilter (nodeID);
  graphEditor.getLassoSelection().deselectAll();

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

CreateConnectionAction::CreateConnectionAction (PMixAudioEngine& audioEngine, uint32 srcFilter, int srcChannel, uint32 dstFilter, int dstChannel) noexcept
: audioEngine(audioEngine)
, srcFilter(srcFilter)
, srcChannel(srcChannel)
, dstFilter(dstFilter)
, dstChannel(dstChannel)
{
}

bool CreateConnectionAction::perform()
{
  return audioEngine.getDoc().addConnection (srcFilter, srcChannel, dstFilter, dstChannel);
}

bool CreateConnectionAction::undo()
{
  audioEngine.getDoc().removeConnection (srcFilter, srcChannel, dstFilter, dstChannel);
  
  return true;
}

int CreateConnectionAction::getSizeInUnits()
{
  return (int) sizeof (*this); //xxx should be more accurate
}