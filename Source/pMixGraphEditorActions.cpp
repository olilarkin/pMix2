/*
  ==============================================================================

    pMixGraphEditorActions.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixGraphEditorActions.h"
#include "pMixPluginWindow.h"

CreateNodeAction::CreateNodeAction (PMixAudioEngine& audioEngine, GraphEditor& graphEditor, const PluginDescription* desc, double x, double y) noexcept
: audioEngine(audioEngine)
, graphEditor(graphEditor)
, x(x)
, y(y)
, desc(*desc)
{
}

bool CreateNodeAction::perform()
{
  nodeID = audioEngine.getDoc().addNode (&desc, x, y);
  
  if (nodeID < NodeID(0xFFFFFFFF))
    return true;
  else
    return false;
  
}

bool CreateNodeAction::undo()
{
  PluginWindow::closeCurrentlyOpenWindowsFor (nodeID);
  audioEngine.getDoc().removeNode(nodeID);
  graphEditor.getLassoSelection().deselectAll();

  return true;
}

int CreateNodeAction::getSizeInUnits()
{
  return (int) sizeof (*this); //xxx should be more accurate
}

RemoveNodeAction::RemoveNodeAction (PMixAudioEngine& audioEngine, GraphEditor& graphEditor, NodeID nodeID) noexcept
: audioEngine(audioEngine)
, graphEditor(graphEditor)
, nodeID(nodeID)
{
  nodeXML = std::unique_ptr<XmlElement>(audioEngine.getDoc().createNodeXml(audioEngine.getGraph().getNodeForId(juce::AudioProcessorGraph::NodeID(nodeID))));
}

bool RemoveNodeAction::perform()
{
  PluginWindow::closeCurrentlyOpenWindowsFor (nodeID);
  
  audioEngine.getDoc().removeNode (nodeID);

  if (nodeID < NodeID(0xFFFFFFFF))
    return true;
  else
    return false;
  
}

bool RemoveNodeAction::undo()
{
  audioEngine.getDoc().createNodeFromXml(*nodeXML);  
  return true;
}

int RemoveNodeAction::getSizeInUnits()
{
  return (int) sizeof (*this); //xxx should be more accurate
}

MoveNodeAction::MoveNodeAction (PMixAudioEngine& audioEngine, GraphEditor& graphEditor, NodeID nodeID, Point<double> startPos, Point<double> endPos) noexcept
: audioEngine(audioEngine)
, graphEditor(graphEditor)
, nodeID(nodeID)
, startPos(startPos)
, endPos(endPos)
{
}

bool MoveNodeAction::perform()
{
  audioEngine.getDoc().setNodePosition (nodeID, endPos.x, endPos.y);
  graphEditor.updateComponents();
  return true;
}

bool MoveNodeAction::undo()
{
  audioEngine.getDoc().setNodePosition (nodeID, startPos.x, startPos.y);
  graphEditor.updateComponents();
  return true;
}

int MoveNodeAction::getSizeInUnits()
{
  return (int) sizeof (*this); //xxx should be more accurate
}

CreateConnectionAction::CreateConnectionAction (PMixAudioEngine& audioEngine, NodeID srcNodeId, int srcChannel, NodeID dstNodeId, int dstChannel) noexcept
: audioEngine(audioEngine)
, srcNodeId(srcNodeId)
, srcChannel(srcChannel)
, dstNodeId(dstNodeId)
, dstChannel(dstChannel)
{
}

bool CreateConnectionAction::perform()
{
  return audioEngine.getDoc().addConnection (srcNodeId, srcChannel, dstNodeId, dstChannel);
}

bool CreateConnectionAction::undo()
{
  audioEngine.getDoc().removeConnection (srcNodeId, srcChannel, dstNodeId, dstChannel);
  
  return true;
}

int CreateConnectionAction::getSizeInUnits()
{
  return (int) sizeof (*this); //xxx should be more accurate
}
