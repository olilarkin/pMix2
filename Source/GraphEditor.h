#ifndef __GRAPHEDITOR_JUCEHEADER__
#define __GRAPHEDITOR_JUCEHEADER__

#include "PMixDocument.h"
#include "ParamView.h"
//#include "ParamTreeView.h"

class FilterComponent;
class ConnectorComponent;
class PinComponent;

#pragma mark -
#pragma mark CreatePluginAction

class CreatePluginAction  : public UndoableAction
{
public:
  CreatePluginAction (PMixDocument& doc, const PluginDescription* desc, double x, double y) noexcept;
  bool perform();
  bool undo();
  int getSizeInUnits();
  
private:
  PMixDocument& doc;
  double x, y;
  const PluginDescription* desc;
  uint32 nodeID;
  JUCE_DECLARE_NON_COPYABLE (CreatePluginAction)
};

#pragma mark -
#pragma mark GraphEditor

class GraphEditor : public Component,
                    public ChangeListener,
                    public LassoSource<Component*>
{
public:
  GraphEditor (PMixDocument& doc);
  ~GraphEditor();

  void paint (Graphics& g);
  void mouseDown (const MouseEvent& e);
  void mouseDrag (const MouseEvent& e);
  void mouseUp (const MouseEvent& e);
  
  void createNewPlugin (const PluginDescription* desc, int x, int y);

  FilterComponent* getComponentForFilter (uint32 filterID) const;
  ConnectorComponent* getComponentForConnection (const AudioProcessorGraph::Connection& conn) const;
  PinComponent* findPinAt (int x, int y) const;

  void resized();
  void changeListenerCallback (ChangeBroadcaster* source);
  void updateComponents();

  
  void beginConnectorDrag (uint32 sourceFilterID, int sourceFilterChannel,
                           uint32 destFilterID, int destFilterChannel,
                           const MouseEvent& e);
  void dragConnector (const MouseEvent& e);
  void endDraggingConnector (const MouseEvent& e);
  
  //LassoSource
  void findLassoItemsInArea (Array <Component*>& results, const Rectangle<int>& area);
  SelectedItemSet<Component*>& getLassoSelection();

private:
  PMixDocument& doc;
  LassoComponent<Component*> lassoComp;
  SelectedItemSet<Component*> selectedItems;
  ScopedPointer<ConnectorComponent> draggingConnector;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphEditor)
};

#pragma mark -
#pragma mark PinComponent

class PinComponent : public Component,
                     public SettableTooltipClient
{
public:
  PinComponent (PMixDocument& doc, const uint32 filterID_, const int index_, const bool isInput_);
  void paint (Graphics& g);
  void mouseDown (const MouseEvent& e);
  void mouseDrag (const MouseEvent& e);
  void mouseUp (const MouseEvent& e);
  
  const uint32 filterID;
  const int index;
  const bool isInput;
  
private:
  PMixDocument& doc;
  
  GraphEditor* getGraphPanel() const noexcept;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PinComponent)
};

#pragma mark -
#pragma mark MovePluginAction

class MovePluginAction  : public UndoableAction
{
public:
  MovePluginAction (PMixDocument& doc, FilterComponent* filterComponent, uint32 nodeID, Point<double> startPos, Point<double> endPos) noexcept;
  bool perform();
  bool undo();
  int getSizeInUnits();
  
private:
  PMixDocument& doc;
  FilterComponent* filterComponent;
  uint32 nodeID;
  Point<double> startPos;
  Point<double> endPos;
  JUCE_DECLARE_NON_COPYABLE (MovePluginAction)
};

#pragma mark -
#pragma mark FilterComponent

class FilterComponent : public Component
{
public:
  FilterComponent (PMixDocument& doc, const uint32 filterID_);
  ~FilterComponent();
  void mouseDown (const MouseEvent& e);
  void mouseDrag (const MouseEvent& e);
  void mouseUp (const MouseEvent& e);
  bool hitTest (int x, int y);
  void paint (Graphics& g);  
  void resized();
  void getPinPos (const int index, const bool isInput, float& x, float& y);
  void update();
  
  PMixDocument& doc;
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
  friend class MovePluginAction;
  
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
  ConnectorComponent (PMixDocument& doc);
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
  PMixDocument& doc;
  float lastInputX, lastInputY, lastOutputX, lastOutputY;
  Path linePath, hitPath;
  bool dragging;
  
  GraphEditor* getGraphPanel() const noexcept;
  void getDistancesFromEnds (int x, int y, double& distanceFromStart, double& distanceFromEnd) const;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConnectorComponent)
};

class ProcessorProgramPropertyComp : public PropertyComponent,
private AudioProcessorListener
{
public:
  ProcessorProgramPropertyComp (const String& name, AudioProcessor& p, int index_);
  ~ProcessorProgramPropertyComp();
  void refresh();
  void audioProcessorChanged (AudioProcessor*);
  void audioProcessorParameterChanged (AudioProcessor*, int, float);
  
private:
  AudioProcessor& owner;
  const int index;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProcessorProgramPropertyComp)
};

#pragma mark -
#pragma mark TooltipBar

class TooltipBar : public Component
                 , private Timer
{
public:
  TooltipBar();
  void paint (Graphics& g);
  void timerCallback();
  
private:
  String tip;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TooltipBar)
};

#pragma mark -
#pragma mark ProgramAudioProcessorEditor

class ProgramAudioProcessorEditor : public AudioProcessorEditor
{
public:
  ProgramAudioProcessorEditor (AudioProcessor* const p);
  void paint (Graphics& g);
  void resized();
  
private:
  PropertyPanel panel;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProgramAudioProcessorEditor)
};

#pragma mark -
#pragma mark PluginWindow

class PluginWindow  : public DocumentWindow
{
public:
  enum WindowFormatType
  {
    Normal = 0,
    Generic,
    Programs,
    Parameters
  };

  PluginWindow (Component* pluginEditor, AudioProcessorGraph::Node*, WindowFormatType);
  ~PluginWindow();

  static PluginWindow* getWindowFor (AudioProcessorGraph::Node*, WindowFormatType);

  static void closeCurrentlyOpenWindowsFor (const uint32 nodeId);
  static void closeAllCurrentlyOpenWindows();

  void moved() override;
  void closeButtonPressed() override;

private:
  AudioProcessorGraph::Node* owner;
  WindowFormatType type;

  float getDesktopScaleFactor() const override     { return 1.0f; }

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginWindow)
};

#endif   // __GraphEditor_JUCEHEADER__
