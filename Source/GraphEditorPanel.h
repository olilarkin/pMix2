#ifndef __GRAPHEDITORPANEL_JUCEHEADER__
#define __GRAPHEDITORPANEL_JUCEHEADER__

#include "FilterGraph.h"
#include "iSpace.h"
#include "ParamView.h"
//#include "ParamTreeView.h"
#include "LookAndFeel.h"

class FilterComponent;
class ConnectorComponent;
class PinComponent;

class CreatePluginAction  : public UndoableAction
{
public:
  CreatePluginAction (FilterGraph& graph, const PluginDescription* desc, double x, double y) noexcept;
  bool perform();
  bool undo();
  int getSizeInUnits();
  
private:
  FilterGraph& graph;
  double x, y;
  const PluginDescription* desc;
  uint32 nodeID;
  JUCE_DECLARE_NON_COPYABLE (CreatePluginAction)
};

class GraphEditorPanel   : public Component,
                           public ChangeListener,
                           public LassoSource<Component*>
{
public:
  GraphEditorPanel (FilterGraph& graph, UndoManager& undoManager);
  ~GraphEditorPanel();

  void paint (Graphics& g);
  void mouseDown (const MouseEvent& e);
  void mouseDrag (const MouseEvent& e);
  void mouseUp (const MouseEvent& e);
  
  void createNewPlugin (const PluginDescription* desc, int x, int y);

  FilterComponent* getComponentForFilter (uint32 filterID) const;
  ConnectorComponent* getComponentForConnection (const AudioProcessorGraph::Connection& conn) const;
  PinComponent* findPinAt (int x, int y) const;

  void resized();
  void changeListenerCallback (ChangeBroadcaster*);
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
  FilterGraph& graph;
  UndoManager& undoManager;
  LassoComponent<Component*> lassoComp;
  SelectedItemSet<Component*> selectedItems;
  ScopedPointer<ConnectorComponent> draggingConnector;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphEditorPanel)
};

class PinComponent : public Component,
                     public SettableTooltipClient
{
public:
  PinComponent (FilterGraph& graph_, const uint32 filterID_, const int index_, const bool isInput_);
  void paint (Graphics& g);
  void mouseDown (const MouseEvent& e);
  void mouseDrag (const MouseEvent& e);
  void mouseUp (const MouseEvent& e);
  
  const uint32 filterID;
  const int index;
  const bool isInput;
  
private:
  FilterGraph& graph;
  
  GraphEditorPanel* getGraphPanel() const noexcept;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PinComponent)
};

class MovePluginAction  : public UndoableAction
{
public:
  MovePluginAction (FilterGraph& graph, FilterComponent* filterComponent, uint32 nodeID, Point<double> startPos, Point<double> endPos) noexcept;
  bool perform();
  bool undo();
  int getSizeInUnits();
  
private:
  FilterGraph& graph;
  FilterComponent* filterComponent;
  uint32 nodeID;
  Point<double> startPos;
  Point<double> endPos;
  JUCE_DECLARE_NON_COPYABLE (MovePluginAction)
};

class FilterComponent : public Component
{
public:
  FilterComponent (FilterGraph& graph_, const uint32 filterID_, UndoManager& undoManager);
  ~FilterComponent();
  void mouseDown (const MouseEvent& e);
  void mouseDrag (const MouseEvent& e);
  void mouseUp (const MouseEvent& e);
  bool hitTest (int x, int y);
  void paint (Graphics& g);  
  void resized();
  void getPinPos (const int index, const bool isInput, float& x, float& y);
  void update();
  
  FilterGraph& graph;
  const uint32 filterID;
  int numInputs, numOutputs;
  
private:
  int pinSize;
  Point<int> originalPos;
  Point<double> endPos;
  Point<double> startPos;
  Font font;
  int numIns, numOuts;
  UndoManager& undoManager;
  bool moving;
  //DropShadowEffect shadow;
  friend class MovePluginAction;
  
  GraphEditorPanel* getGraphPanel() const noexcept;
  
  FilterComponent (const FilterComponent&);
  FilterComponent& operator= (const FilterComponent&);
};

#pragma mark -
#pragma mark ConnectorComponent

class ConnectorComponent   : public Component
                           , public SettableTooltipClient
{
public:
  ConnectorComponent (FilterGraph& graph_);
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
  FilterGraph& graph;
  float lastInputX, lastInputY, lastOutputX, lastOutputY;
  Path linePath, hitPath;
  bool dragging;
  
  GraphEditorPanel* getGraphPanel() const noexcept;
  void getDistancesFromEnds (int x, int y, double& distanceFromStart, double& distanceFromEnd) const;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConnectorComponent)
};

class GraphDocumentComponent  : public Component
{
public:
  
  GraphDocumentComponent (AudioPluginFormatManager& formatManager, AudioDeviceManager* deviceManager);
  ~GraphDocumentComponent();
  
  void createNewPlugin (const PluginDescription* desc, int x, int y);  
  void resized();
  void setZoom (double scale);
  double getZoom() const;

  FilterGraph graph;
  UndoManager undoManager;

private:
  
  AudioDeviceManager* deviceManager;
  AudioProcessorPlayer graphPlayer;
  MidiKeyboardState keyState;

  GraphEditorPanel* graphPanel;
  Component* keyboardComp;
  Component* statusBar;

  pMixLookAndFeel lf;
  iSpaceComponent* iSpace;
  ParamView* paramView;
  
  StretchableLayoutManager verticalLayout;
  StretchableLayoutResizerBar* verticalDividerBar;
  
//  ParamTreeView* treeView;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphDocumentComponent)
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

#endif   // __GRAPHEDITORPANEL_JUCEHEADER__
