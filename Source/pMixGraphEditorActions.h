/*
  ==============================================================================

    pMixGraphEditorActions.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#pragma once

#include "pMixAudioEngine.h"
#include "pMixGraphEditorParts.h"

class NodeComponent;
class ConnectorComponent;
class PinComponent;

#pragma mark -
#pragma mark UndoableActions

class CreateNodeAction  : public UndoableAction
{
public:
  CreateNodeAction (PMixAudioEngine& audioEngine, GraphEditor& graphEditor, const PluginDescription* desc, double x, double y) noexcept;
  bool perform();
  bool undo();
  int getSizeInUnits();
  
private:
  PMixAudioEngine& audioEngine;
  GraphEditor& graphEditor;
  double x, y;
  PluginDescription desc;
  NodeID nodeID;
  JUCE_DECLARE_NON_COPYABLE (CreateNodeAction)
};

class RemoveNodeAction  : public UndoableAction
{
public:
  RemoveNodeAction (PMixAudioEngine& audioEngine, GraphEditor& graphEditor, NodeID nodeID) noexcept;
  bool perform();
  bool undo();
  int getSizeInUnits();
  
private:
  PMixAudioEngine& audioEngine;
  GraphEditor& graphEditor;
  NodeID nodeID;
  std::unique_ptr<XmlElement> nodeXML;
  JUCE_DECLARE_NON_COPYABLE (RemoveNodeAction)
};

class MoveNodeAction  : public UndoableAction
{
public:
  MoveNodeAction (PMixAudioEngine& audioEngine, GraphEditor& graphEditor, NodeID nodeID, Point<double> startPos, Point<double> endPos) noexcept;
  bool perform();
  bool undo();
  int getSizeInUnits();
  
private:
  PMixAudioEngine& audioEngine;
  GraphEditor& graphEditor;
  NodeID nodeID;
  Point<double> startPos;
  Point<double> endPos;
  JUCE_DECLARE_NON_COPYABLE (MoveNodeAction)
};

class CreateConnectionAction  : public UndoableAction
{
public:
  CreateConnectionAction (PMixAudioEngine& audioEngine, NodeID srcNodeId, int srcChannel, NodeID dstNodeId, int dstChannel) noexcept;
  bool perform();
  bool undo();
  int getSizeInUnits();
  
private:
  PMixAudioEngine& audioEngine;
  NodeID srcNodeId;
  int srcChannel;
  NodeID dstNodeId;
  int dstChannel;
  JUCE_DECLARE_NON_COPYABLE (CreateConnectionAction)
};

