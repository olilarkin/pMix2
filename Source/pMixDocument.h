/*
  ==============================================================================

    pMixDocument.h
    Author:  Oliver Larkin
 
  ==============================================================================
*/

#ifndef PMIXDOCUMENT_H_INCLUDED
#define PMIXDOCUMENT_H_INCLUDED

#include "JuceHeader.h"
#include "pMixDefaultColours.h"
#include "pMixConstants.h"

const char* const filenameSuffix = ".pmix";
const char* const filenameWildcard = "*.pmix";

class PMixAudioEngine;

class PMixDocument : public FileBasedDocument
                   , public Timer

{
public:
  PMixDocument (PMixAudioEngine& audioEngine);
  ~PMixDocument();
  
  void timerCallback();

  void addPreset(const uint32 nodeId, double x, double y);
  void setPresetPosition(const uint32 nodeId, const int presetIdx, double x, double y);
  void getPresetPosition(const uint32 nodeId, const int presetIdx, double& x, double& y) const;
  double getPresetWeight(const uint32 nodeId, const int presetIdx);
  void removePreset(const uint32 nodeId, const int presetIdx);
  void setPresetName(const uint32 nodeId, const int presetIdx, String newName);

  void setFilterIPos(const uint32 nodeId, double x, double y);
  void getFilterIPos(const uint32 nodeId, double& x, double& y) const;
  void updateCoefficients(const AudioProcessorGraph::Node::Ptr node);

  void setFilterColour(const uint32 nodeId, const Colour colour);
  Colour getFilterColour(const uint32 nodeId) const;
  
  int getNumFilters() const noexcept;
  const AudioProcessorGraph::Node::Ptr getNode (const int index) const noexcept;
  const AudioProcessorGraph::Node::Ptr getNodeForId (const uint32 uid) const noexcept;
  
  uint32 addFilter (const PluginDescription* desc, double x, double y);
  void removeFilter (const uint32 filterUID);
  void disconnectFilter (const uint32 filterUID);
  
  bool getParameterIsInterpolated(const uint32 nodeId, const int paramIdx);
  void setParameterToInterpolate(const uint32 nodeId, const int paramIdx, bool interpolate);

  void removeIllegalConnections();
  void setNodePosition (const uint32 nodeId, double x, double y);
  void getNodePosition (const uint32 nodeId, double& x, double& y) const;
  int getNumConnections() const noexcept;
  const AudioProcessorGraph::Connection* getConnection (const int index) const noexcept;
  const AudioProcessorGraph::Connection* getConnectionBetween (uint32 sourceFilterUID, int sourceFilterChannel, uint32 destFilterUID, int destFilterChannel) const noexcept;
  bool canConnect (uint32 sourceFilterUID, int sourceFilterChannel, uint32 destFilterUID, int destFilterChannel) const noexcept;
  bool addConnection (uint32 sourceFilterUID, int sourceFilterChannel, uint32 destFilterUID, int destFilterChannel);
  void removeConnection (const int index);
  void removeConnection (uint32 sourceFilterUID, int sourceFilterChannel, uint32 destFilterUID, int destFilterChannel);
  void clear();
  
  XmlElement* createXml() const;
  void createNodeFromXml (XmlElement& xml, const String& newSourceCode = String::empty);
  void restoreFromXml (const XmlElement& xml);
  static XmlElement* createNodeXml (AudioProcessorGraph::Node* const node) noexcept;

  String getDocumentTitle();
  Result loadDocument (const File& file);
  Result saveDocument (const File& file);

  File getLastDocumentOpened();
  void setLastDocumentOpened (const File& file);
  // The special channel index used to refer to a filter's midi channel
  static const int midiChannelNumber;
  
  void setSnappingGrid (const int numPixels, const bool active, const bool shown);
  
  int getSnappingGridSize() const noexcept                                { return snapGridPixels; }
  bool isSnapActive (const bool disableIfCtrlKeyDown) const noexcept;
  bool isSnapShown() const noexcept                                       { return snapShown; }
  
  int snapPosition (int pos) const noexcept;
  void setComponentOverlayOpacity (const float alpha);
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

  int snapGridPixels;
  bool snapActive, snapShown;
  float componentOverlayOpacity;
  PMixDefaultColours defaultColours;
  const File drawPath;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PMixDocument)
};


#endif   // PMIXDOCUMENT_H_INCLUDED
