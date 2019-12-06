/*
  ==============================================================================

    pMixGraphEditorParts.h
    based on JUCE demo host
    Author:  Oliver Larkin

  ==============================================================================
*/

#pragma once

#include "pMixGraphEditor.h"
#include "pMixGenericEditor.h"

class GraphEditor;

class PinComponent : public Component
                   , public SettableTooltipClient
{
public:
  PinComponent (PMixAudioEngine& audio, NodeID nodeID, const int index_, const bool isInput_);
  void paint (Graphics& g) override;
  void mouseDown (const MouseEvent& e) override;
  void mouseDrag (const MouseEvent& e) override;
  void mouseUp (const MouseEvent& e) override;
  void mouseEnter (const MouseEvent& e) override;
  void mouseExit (const MouseEvent& e) override;

  NodeID nodeID;
  const int index;
  const bool isInput;
  bool mouseOver;
  int busIdx;
  
private:
  PMixAudioEngine& audioEngine;

  GraphEditor* getGraphEditor() const noexcept;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PinComponent)
};

#pragma mark -
#pragma mark NodeComponent

class NodeComponent : public Component
                    , public ChangeListener
{
public:
  NodeComponent (PMixAudioEngine& audioEngine, NodeID nodeID);
  ~NodeComponent();
  void mouseDown (const MouseEvent& e) override;
  void mouseDrag (const MouseEvent& e) override;
  void mouseUp (const MouseEvent& e) override;
  void mouseDoubleClick (const MouseEvent&) override;
  
  bool hitTest (int x, int y) override;
  void paint (Graphics& g) override;
  void resized() override;
  
  void getPinPos (const int index, const bool isInput, float& x, float& y);
  void update();
  
  void removeEditor();
  
  void changeListenerCallback (ChangeBroadcaster* source) override;
  
  void bubbleMessage(String msg);
  
public:
  PMixAudioEngine& audioEngine;
  NodeID nodeID;
  int numInputs, numOutputs;
  
private:
  int pinSize;
  Point<int> originalPos;
  Point<double> endPos;
  Point<double> startPos;
  Font font;
  int numIns, numOuts;
  bool moving;
  //DropShadowEffect shadow;
  friend class MoveNodeAction;
  PMixGenericAudioProcessorEditor* editor;
  Label* nodeName;
  bool highlight;
  GraphEditor* getGraphEditor() const noexcept;
  
  NodeComponent (const NodeComponent&);
  NodeComponent& operator= (const NodeComponent&);
  BubbleMessageComponent bbl;
};

#pragma mark -
#pragma mark ConnectorComponent

class ConnectorComponent   : public Component
, public SettableTooltipClient
{
public:
  ConnectorComponent (PMixAudioEngine& audioEngine);
  void setInput (NodeID sourceNodeId_, const int sourceNodeChannel_);
  void setOutput (NodeID destNodeId_, const int destNodeChannel_);
  void dragStart (int x, int y);
  void dragEnd (int x, int y);
  void update();
  void resizeToFit();
  void getPoints (float& x1, float& y1, float& x2, float& y2) const;
  void paint (Graphics& g);
  bool hitTest (int x, int y);
  void mouseDown (const MouseEvent&);
  void mouseDrag (const MouseEvent& e);
  void mouseUp (const MouseEvent& e);
  void resized();
  
  NodeID sourceNodeId, destNodeId;
  int sourceNodeChannel, destNodeChannel;
  
private:
  PMixAudioEngine& audioEngine;
  float lastInputX, lastInputY, lastOutputX, lastOutputY;
  Path linePath, hitPath;
  bool dragging;
  
  GraphEditor* getGraphEditor() const noexcept;
  void getDistancesFromEnds (int x, int y, double& distanceFromStart, double& distanceFromEnd) const;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConnectorComponent)
};

