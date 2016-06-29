/*
  ==============================================================================

    pMixGraphEditorParts.h
    based on JUCE demo host
    Author:  Oliver Larkin

  ==============================================================================
*/

#ifndef PMIXGRAPHEDITORPARTS_H_INCLUDED
#define PMIXGRAPHEDITORPARTS_H_INCLUDED

#include "pMixGraphEditor.h"
#include "pMixGenericEditor.h"

class GraphEditor;

class PinComponent : public Component
                   , public SettableTooltipClient
{
public:
  PinComponent (PMixAudioEngine& audio, const uint32 nodeId_, const int index_, const bool isInput_);
  void paint (Graphics& g) override;
  void mouseDown (const MouseEvent& e) override;
  void mouseDrag (const MouseEvent& e) override;
  void mouseUp (const MouseEvent& e) override;
  void mouseEnter (const MouseEvent& e) override;
  void mouseExit (const MouseEvent& e) override;

  const uint32 nodeId;
  const int index;
  const bool isInput;
  bool mouseOver;

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
  NodeComponent (PMixAudioEngine& audioEngine, const uint32 nodeId_);
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
  const uint32 nodeId;
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
};

#pragma mark -
#pragma mark ConnectorComponent

class ConnectorComponent   : public Component
, public SettableTooltipClient
{
public:
  ConnectorComponent (PMixAudioEngine& audioEngine);
  void setInput (const uint32 sourceNodeId_, const int sourceNodeChannel_);
  void setOutput (const uint32 destNodeId_, const int destNodeChannel_);
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
  
  uint32 sourceNodeId, destNodeId;
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

#endif  // PMIXGRAPHEDITORPARTS_H_INCLUDED
