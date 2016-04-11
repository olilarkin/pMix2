/*
  ==============================================================================

    pMixGraphEditorActions.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#ifndef PMIXGRAPHEDITORACTIONS_H_INCLUDED
#define PMIXGRAPHEDITORACTIONS_H_INCLUDED

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
  uint32 nodeId;
  JUCE_DECLARE_NON_COPYABLE (CreateNodeAction)
};

class RemoveNodeAction  : public UndoableAction
{
public:
  RemoveNodeAction (PMixAudioEngine& audioEngine, GraphEditor& graphEditor, const uint32 nodeId) noexcept;
  bool perform();
  bool undo();
  int getSizeInUnits();
  
private:
  PMixAudioEngine& audioEngine;
  GraphEditor& graphEditor;
  uint32 nodeId;
  ScopedPointer<XmlElement> nodeXML;
  JUCE_DECLARE_NON_COPYABLE (RemoveNodeAction)
};

class MoveNodeAction  : public UndoableAction
{
public:
  MoveNodeAction (PMixAudioEngine& audioEngine, GraphEditor& graphEditor, const uint32 nodeId, Point<double> startPos, Point<double> endPos) noexcept;
  bool perform();
  bool undo();
  int getSizeInUnits();
  
private:
  PMixAudioEngine& audioEngine;
  GraphEditor& graphEditor;
  uint32 nodeId;
  Point<double> startPos;
  Point<double> endPos;
  JUCE_DECLARE_NON_COPYABLE (MoveNodeAction)
};

class CreateConnectionAction  : public UndoableAction
{
public:
  CreateConnectionAction (PMixAudioEngine& audioEngine, uint32 srcNodeId, int srcChannel, uint32 dstNodeId, int dstChannel) noexcept;
  bool perform();
  bool undo();
  int getSizeInUnits();
  
private:
  PMixAudioEngine& audioEngine;
  uint32 srcNodeId;
  int srcChannel;
  uint32 dstNodeId;
  int dstChannel;
  JUCE_DECLARE_NON_COPYABLE (CreateConnectionAction)
};

#endif  // PMIXGRAPHEDITORACTIONS_H_INCLUDED
