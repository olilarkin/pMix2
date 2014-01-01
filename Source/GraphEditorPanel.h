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

#pragma mark -
#pragma mark GraphEditorPanel

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
  
  class CreatePluginAction  : public UndoableAction
  {
  public:
    CreatePluginAction (FilterGraph& graph, const PluginDescription* desc, double x, double y) noexcept
    : graph(graph)
    , x(x)
    , y(y)
    , desc(desc)
    {
    }
    
    bool perform()
    {
      nodeID = graph.addFilter (desc, x, y);

      return true;
    }
    
    bool undo()
    {
      graph.removeFilter(nodeID);
      
      return true;
    }
    
    int getSizeInUnits()
    {
      return (int) sizeof (*this); //xxx should be more accurate
    }

  private:
    FilterGraph& graph;
    double x, y;
    const PluginDescription* desc;
    uint32 nodeID;
    JUCE_DECLARE_NON_COPYABLE (CreatePluginAction)
  };
  
  FilterGraph& graph;
  UndoManager& undoManager;
  LassoComponent<Component*> lassoComp;
  SelectedItemSet<Component*> selectedItems;
  ScopedPointer<ConnectorComponent> draggingConnector;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphEditorPanel)
};

class GraphDocumentComponent  : public Component
{
public:
  
  GraphDocumentComponent (AudioPluginFormatManager& formatManager, AudioDeviceManager* deviceManager);
  ~GraphDocumentComponent();
  
  void createNewPlugin (const PluginDescription* desc, int x, int y);  
  void resized();

  FilterGraph graph;
  UndoManager undoManager;

private:
  
  AudioDeviceManager* deviceManager;
  AudioProcessorPlayer graphPlayer;
  MidiKeyboardState keyState;

  GraphEditorPanel* graphPanel;
  Component* keyboardComp;
  Component* statusBar;

  LookAndFeelCustom lf;
  iSpaceComponent* iSpace;
  ParamView* paramView;
  
  StretchableLayoutManager verticalLayout;
  StretchableLayoutResizerBar* verticalDividerBar;
  
//  ParamTreeView* treeView;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphDocumentComponent)
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
