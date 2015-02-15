/*
  ==============================================================================

    pMixGraphEditorParts.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#ifndef PMIXGRAPHEDITORPARTS_H_INCLUDED
#define PMIXGRAPHEDITORPARTS_H_INCLUDED

#include "pMixGraphEditor.h"


class GraphEditor;

class PinComponent : public Component,
public SettableTooltipClient
{
public:
  PinComponent (PMixAudioEngine& audio, const uint32 filterID_, const int index_, const bool isInput_);
  void paint (Graphics& g);
  void mouseDown (const MouseEvent& e);
  void mouseDrag (const MouseEvent& e);
  void mouseUp (const MouseEvent& e);
  
  const uint32 filterID;
  const int index;
  const bool isInput;
  
private:
  PMixAudioEngine& audioEngine;
  
  GraphEditor* getGraphPanel() const noexcept;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PinComponent)
};

#pragma mark -
#pragma mark FilterComponent

class FilterComponent : public Component
{
public:
  FilterComponent (PMixAudioEngine& audioEngine, const uint32 filterID_);
  ~FilterComponent();
  void mouseDown (const MouseEvent& e);
  void mouseDrag (const MouseEvent& e);
  void mouseUp (const MouseEvent& e);
  bool hitTest (int x, int y);
  void paint (Graphics& g);
  void resized();
  void getPinPos (const int index, const bool isInput, float& x, float& y);
  void update();
  
  PMixAudioEngine& audioEngine;
  const uint32 filterID;
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
  friend class MoveFilterAction;
  
  GraphEditor* getGraphPanel() const noexcept;
  
  FilterComponent (const FilterComponent&);
  FilterComponent& operator= (const FilterComponent&);
};

#pragma mark -
#pragma mark ConnectorComponent

class ConnectorComponent   : public Component
, public SettableTooltipClient
{
public:
  ConnectorComponent (PMixAudioEngine& audioEngine);
  void setInput (const uint32 sourceFilterID_, const int sourceFilterChannel_);
  void setOutput (const uint32 destFilterID_, const int destFilterChannel_);
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
  
  uint32 sourceFilterID, destFilterID;
  int sourceFilterChannel, destFilterChannel;
  
private:
  PMixAudioEngine& audioEngine;
  float lastInputX, lastInputY, lastOutputX, lastOutputY;
  Path linePath, hitPath;
  bool dragging;
  
  GraphEditor* getGraphPanel() const noexcept;
  void getDistancesFromEnds (int x, int y, double& distanceFromStart, double& distanceFromEnd) const;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConnectorComponent)
};

#endif  // PMIXGRAPHEDITORPARTS_H_INCLUDED
