/*
  ==============================================================================

    pMixGraphEditor.h
    based on JUCE demo host
    Author:  Oliver Larkin

  ==============================================================================
*/

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
                    public ApplicationCommandTarget,
                    public FileDragAndDropTarget
{
public:
  GraphEditor (PMixAudioEngine& audioEngine);
  ~GraphEditor();

  void paint (Graphics& g) override;
  void mouseDown (const MouseEvent& e) override;
  void mouseDrag (const MouseEvent& e) override;
  void mouseUp (const MouseEvent& e) override;
  void mouseDoubleClick (const MouseEvent&) override;

  void createNewFilter (const PluginDescription* desc, int x, int y);

  FilterComponent* getComponentForFilter (uint32 nodeId) const;
  ConnectorComponent* getComponentForConnection (const AudioProcessorGraph::Connection& conn) const;
  PinComponent* findPinAt (int x, int y) const;

  void resized() override;
  void changeListenerCallback (ChangeBroadcaster* source) override;
  
  void updateComponents();
  void clear();
  
  void beginConnectorDrag (uint32 sourceFilterID, int sourceFilterChannel,
                           uint32 destFilterID, int destFilterChannel,
                           const MouseEvent& e);
  void dragConnector (const MouseEvent& e);
  void endDraggingConnector (const MouseEvent& e);

  
  void deleteSelection();
  void selectAll();

  void updateFaustNode (uint32 nodeID, String& newSourceCode);
  
  //LassoSource
  void findLassoItemsInArea (Array <Component*>& results, const Rectangle<int>& area) override;
  SelectedItemSet<Component*>& getLassoSelection() override;
  
  //ApplicationCommandTarget
  ApplicationCommandTarget* getNextCommandTarget() override;
  void getAllCommands (Array <CommandID>& commands) override;
  void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result) override;
  bool perform (const InvocationInfo& info) override;
  
  // FileDragAndDropTarget
  bool isInterestedInFileDrag (const StringArray& files) override;
  void fileDragEnter (const StringArray& files, int x, int y) override;
  void fileDragMove (const StringArray& files, int x, int y) override;
  void fileDragExit (const StringArray& files) override;
  void filesDropped (const StringArray& files, int x, int y) override;

private:
  PMixAudioEngine& audioEngine;
  LassoComponent<Component*> lassoComp;
  SelectedItemSet<Component*> selectedItems;
  ScopedPointer<ConnectorComponent> draggingConnector;
  bool somethingIsBeingDraggedOver;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphEditor)
};

#endif   // __GraphEditor_JUCEHEADER__
