#ifndef __GRAPHEDITOR_JUCEHEADER__
#define __GRAPHEDITOR_JUCEHEADER__

#include "pMixAudioEngine.h"
#include "pMixPluginWindow.h"
#include "pMixGraphEditorParts.h"

class FilterComponent;
class ConnectorComponent;
class PinComponent;

class GraphEditor : public Component,
                    public ChangeListener,
                    public LassoSource<Component*>,
                    public ChangeBroadcaster,
                    public ApplicationCommandTarget
{
public:
  GraphEditor (PMixAudioEngine& audioEngine);
  ~GraphEditor();

  void paint (Graphics& g);
  void mouseDown (const MouseEvent& e);
  void mouseDrag (const MouseEvent& e);
  void mouseUp (const MouseEvent& e);
  
  void createNewFilter (const PluginDescription* desc, int x, int y);

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

  void updateFaustNode (const int nodeID, String& newSourceCode);
  
  //LassoSource
  void findLassoItemsInArea (Array <Component*>& results, const Rectangle<int>& area);
  SelectedItemSet<Component*>& getLassoSelection();
  
  //ApplicationCommandTarget
  ApplicationCommandTarget* getNextCommandTarget();
  void getAllCommands (Array <CommandID>& commands);
  void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result);
  bool perform (const InvocationInfo& info);

private:
  PMixAudioEngine& audioEngine;
  LassoComponent<Component*> lassoComp;
  SelectedItemSet<Component*> selectedItems;
  ScopedPointer<ConnectorComponent> draggingConnector;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphEditor)
};

#endif   // __GraphEditor_JUCEHEADER__
