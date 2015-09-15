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

class FilterComponent;
class ConnectorComponent;
class PinComponent;

#pragma mark -
#pragma mark UndoableActions

class CreateFilterAction  : public UndoableAction
{
public:
  CreateFilterAction (PMixAudioEngine& audioEngine, const PluginDescription* desc, double x, double y) noexcept;
  bool perform();
  bool undo();
  int getSizeInUnits();
  
private:
  PMixAudioEngine& audioEngine;
  double x, y;
  const PluginDescription* desc;
  uint32 nodeID;
  JUCE_DECLARE_NON_COPYABLE (CreateFilterAction)
};

class RemoveFilterAction  : public UndoableAction
{
public:
  RemoveFilterAction (PMixAudioEngine& audioEngine, uint32 nodeID) noexcept;
  bool perform();
  bool undo();
  int getSizeInUnits();
  
private:
  PMixAudioEngine& audioEngine;
  uint32 nodeID;
  ScopedPointer<XmlElement> nodeXML;
  JUCE_DECLARE_NON_COPYABLE (RemoveFilterAction)
};

class MoveFilterAction  : public UndoableAction
{
public:
  MoveFilterAction (PMixAudioEngine& audioEngine, GraphEditor& graphEditor, uint32 nodeID, Point<double> startPos, Point<double> endPos) noexcept;
  bool perform();
  bool undo();
  int getSizeInUnits();
  
private:
  PMixAudioEngine& audioEngine;
  GraphEditor& graphEditor;
  uint32 nodeID;
  Point<double> startPos;
  Point<double> endPos;
  JUCE_DECLARE_NON_COPYABLE (MoveFilterAction)
};

class CreateConnectionAction  : public UndoableAction
{
public:
  CreateConnectionAction (PMixAudioEngine& audioEngine, uint32 srcFilter, int srcChannel, uint32 dstFilter, int dstChannel) noexcept;
  bool perform();
  bool undo();
  int getSizeInUnits();
  
private:
  PMixAudioEngine& audioEngine;
  uint32 srcFilter;
  int srcChannel;
  uint32 dstFilter;
  int dstChannel;
  JUCE_DECLARE_NON_COPYABLE (CreateConnectionAction)
};

#endif  // PMIXGRAPHEDITORACTIONS_H_INCLUDED
