/*
  ==============================================================================

    pMixDocument.h
    Author:  Oliver Larkin
 
  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "pMixDefaultColours.h"
#include "pMixConstants.h"

class PMixAudioEngine;

class PMixDocument : public FileBasedDocument
                   , public Timer

{
public:
  PMixDocument (PMixAudioEngine& audioEngine);
  ~PMixDocument();
  
  void timerCallback();

  DynamicObject* getPresetWithUID(NodeID nodeID, int presetId) const;
  void addPreset(NodeID nodeID, double x, double y);
  void setPresetPosition(NodeID nodeID, int presetId, double x, double y);
  void getPresetPosition(NodeID nodeID, int presetId, double& x, double& y) const;
  double getPresetWeight(NodeID nodeID, int presetId);
  void removePreset(NodeID nodeID, int presetId);
  void setPresetName(NodeID nodeID, int presetId, String newName);
  int getNumPresetsForNode(NodeID nodeID);

  void setNodeIPos(NodeID nodeID, double x, double y);
  void getNodeIPos(NodeID nodeID, double& x, double& y) const;
  void updateCoefficients(const AudioProcessorGraph::Node::Ptr node);

  void setNodeColour(NodeID nodeID, const Colour& colour);
  Colour getNodeColour(NodeID nodeID) const;
  
  int getNumNodes() const noexcept;
  const AudioProcessorGraph::Node::Ptr getNode (int index) const noexcept;
  const AudioProcessorGraph::Node::Ptr getNodeForId (NodeID nodeID) const noexcept;
  
  NodeID addNode(const PluginDescription* desc, double x, double y);
  void removeNode(NodeID nodeID);
  void disconnectNode(NodeID nodeID);
  
  void setNodeUIStatus(NodeID nodeID, uint32 uiStatus);
  
  bool getParameterIsInterpolated(NodeID nodeID, int paramIdx);
  void setParameterToInterpolate(NodeID nodeID, int paramIdx, bool interpolate);

  void removeIllegalConnections();
  void setNodePosition(NodeID nodeID, double x, double y);
  void getNodePosition(NodeID nodeID, double& x, double& y) const;
  int getNumConnections() const noexcept;
  const AudioProcessorGraph::Connection getConnection (int index) const noexcept;
  bool isConnected(NodeID sourceNodeId, int sourceNodeChannel, NodeID destNodeId, int destNodeChannel) const noexcept;
  bool canConnect(NodeID sourceNodeId, int sourceNodeChannel, NodeID destNodeId, int destNodeChannel) const noexcept;
  bool addConnection(NodeID sourceNodeId, int sourceNodeChannel, NodeID destNodeId, int destNodeChannel);
  void removeConnection(int index);
  void removeConnection(NodeID sourceNodeId, int sourceNodeChannel, NodeID destNodeId, int destNodeChannel);
  void clear();
  
  std::unique_ptr<XmlElement> createXml() const;
  void createNodeFromXml(XmlElement& xml, const String& newSourceCode = String());
  void restoreFromXml(const XmlElement& xml);
  static XmlElement* createNodeXml (AudioProcessorGraph::Node* const node) noexcept;

  String getDocumentTitle();
  Result loadDocument(const File& file);
  Result saveDocument(const File& file);

  File getLastDocumentOpened();
  void setLastDocumentOpened (const File& file);
  // The special channel index used to refer to a node's midi channel
  static const int midiChannelNumber;
  
  void setSnappingGrid (int numPixels, bool active, bool shown);
  
  int getSnappingGridSize() const noexcept                                { return snapGridPixels; }
  bool isSnapActive(bool disableIfCtrlKeyDown) const noexcept;
  bool isSnapShown() const noexcept                                       { return snapShown; }
  
  int snapPosition (int pos) const noexcept;
  void setComponentOverlayOpacity (float alpha);
  float getComponentOverlayOpacity() const noexcept                       { return componentOverlayOpacity; }
  
  void beginTransaction();
  void beginTransaction (const String& name);
  bool perform (UndoableAction* const action, const String& actionName);

  UndoManager& getUndoManager() noexcept                                  { return undoManager; }

  void initialize();
  
  String getLibraryPath();
  
private:
  UndoManager undoManager;
  PMixAudioEngine& audioEngine;
  uint32 lastUID;
  uint32 getNextUID() noexcept;

  int lastPresetUID;
  int getNextPresetUID() noexcept;
  
  int snapGridPixels;
  bool snapActive, snapShown;
  float componentOverlayOpacity;
  PMixDefaultColours defaultColours;
  const File drawPath;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PMixDocument)
};


