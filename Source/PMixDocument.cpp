#include "pMixDocument.h"
#include "pMixAudioEngine.h"

const int PMixDocument::midiChannelNumber = 0x1000;

PMixDocument::PMixDocument (PMixAudioEngine& audioEngine)
: FileBasedDocument (filenameSuffix,
                     filenameWildcard,
                     "Load a pMix patch",
                     "Save a pMix patch"),
                     audioEngine(audioEngine),
                     lastUID (0),
                     snapGridPixels (8),
                     snapActive (true),
                     snapShown (true),
                     componentOverlayOpacity (0.33f)
{
}

PMixDocument::~PMixDocument()
{
  audioEngine.getGraph().clear();
}

uint32 PMixDocument::getNextUID() noexcept
{
  return ++lastUID;
}

int PMixDocument::getNumFilters() const noexcept
{
  return audioEngine.getGraph().getNumNodes();
}

const AudioProcessorGraph::Node::Ptr PMixDocument::getNode (const int index) const noexcept
{
  return audioEngine.getGraph().getNode (index);
}

const AudioProcessorGraph::Node::Ptr PMixDocument::getNodeForId (const uint32 uid) const noexcept
{
  return audioEngine.getGraph().getNodeForId (uid);
}

uint32 PMixDocument::addFilter (const PluginDescription* desc, double x, double y)
{
  AudioProcessorGraph::Node* node = nullptr;

  if (desc != nullptr)
  {
    String errorMessage;

    if (AudioPluginInstance* instance = audioEngine.getFormatManager().createPluginInstance (*desc, audioEngine.getGraph().getSampleRate(), audioEngine.getGraph().getBlockSize(), errorMessage))
      node = audioEngine.getGraph().addNode (instance);

    if (node != nullptr)
    {
      node->properties.set ("x", x);
      node->properties.set ("y", y);
      node->properties.set ("uiLastX", 0);
      node->properties.set ("uiLastY", 0);
      changed();
    }
    else
    {
      AlertWindow::showMessageBox (AlertWindow::WarningIcon,
                                   TRANS("Couldn't create filter"),
                                   errorMessage);
      
      return 0xFFFFFFFF;
    }
  }
  
  return node->nodeId;
}

void PMixDocument::removeFilter (const uint32 id)
{
  if (audioEngine.getGraph().removeNode (id))
    changed();
}

void PMixDocument::disconnectFilter (const uint32 id)
{
  if (audioEngine.getGraph().disconnectNode (id))
    changed();
}

void PMixDocument::removeIllegalConnections()
{
  if (audioEngine.getGraph().removeIllegalConnections())
    changed();
}

void PMixDocument::setNodePosition (const int nodeId, double x, double y)
{
  const AudioProcessorGraph::Node::Ptr n (audioEngine.getGraph().getNodeForId (nodeId));

  if (n != nullptr)
  {
    n->properties.set ("x", jlimit (0.0, 1.0, x));
    n->properties.set ("y", jlimit (0.0, 1.0, y));
  }
}

void PMixDocument::getNodePosition (const int nodeId, double& x, double& y) const
{
  x = y = 0;

  const AudioProcessorGraph::Node::Ptr n (audioEngine.getGraph().getNodeForId (nodeId));

  if (n != nullptr)
  {
    x = (double) n->properties ["x"];
    y = (double) n->properties ["y"];
  }
}


int PMixDocument::getNumConnections() const noexcept
{
  return audioEngine.getGraph().getNumConnections();
}

const AudioProcessorGraph::Connection* PMixDocument::getConnection (const int index) const noexcept
{
  return audioEngine.getGraph().getConnection (index);
}

const AudioProcessorGraph::Connection* PMixDocument::getConnectionBetween (uint32 sourceFilterUID, int sourceFilterChannel,
    uint32 destFilterUID, int destFilterChannel) const noexcept
{
  return audioEngine.getGraph().getConnectionBetween (sourceFilterUID, sourceFilterChannel,
                                     destFilterUID, destFilterChannel);
}

bool PMixDocument::canConnect (uint32 sourceFilterUID, int sourceFilterChannel,
                              uint32 destFilterUID, int destFilterChannel) const noexcept
{
  return audioEngine.getGraph().canConnect (sourceFilterUID, sourceFilterChannel,
                           destFilterUID, destFilterChannel);
}

bool PMixDocument::addConnection (uint32 sourceFilterUID, int sourceFilterChannel,
                                 uint32 destFilterUID, int destFilterChannel)
{
  const bool result = audioEngine.getGraph().addConnection (sourceFilterUID, sourceFilterChannel,
                      destFilterUID, destFilterChannel);

  if (result)
    changed();

  return result;
}

void PMixDocument::removeConnection (const int index)
{
  audioEngine.getGraph().removeConnection (index);
  changed();
}

void PMixDocument::removeConnection (uint32 sourceFilterUID, int sourceFilterChannel,
                                    uint32 destFilterUID, int destFilterChannel)
{
  if (audioEngine.getGraph().removeConnection (sourceFilterUID, sourceFilterChannel,
                              destFilterUID, destFilterChannel))
    changed();
}

void PMixDocument::clear()
{
  //PluginWindow::closeAllCurrentlyOpenWindows();

  audioEngine.getGraph().clear();
  changed();
}

String PMixDocument::getDocumentTitle()
{
  if (! getFile().exists())
    return "Unnamed";

  return getFile().getFileNameWithoutExtension();
}

Result PMixDocument::loadDocument (const File& file)
{
  XmlDocument doc (file);
  ScopedPointer<XmlElement> xml (doc.getDocumentElement());

  if (xml == nullptr || ! xml->hasTagName ("PMIXDOC"))
    return Result::fail ("Not a valid pMix file");

  restoreFromXml (*xml);
  return Result::ok();
}

Result PMixDocument::saveDocument (const File& file)
{
  ScopedPointer<XmlElement> xml (createXml());

  if (! xml->writeToFile (file, String::empty))
    return Result::fail ("Couldn't write to the file");

  return Result::ok();
}

File PMixDocument::getLastDocumentOpened()
{
  RecentlyOpenedFilesList recentFiles;
  recentFiles.restoreFromString (audioEngine.getAppProperties().getUserSettings()
                                 ->getValue ("recentPMixDocumentFiles"));

  return recentFiles.getFile (0);
}

void PMixDocument::setLastDocumentOpened (const File& file)
{
  RecentlyOpenedFilesList recentFiles;
  recentFiles.restoreFromString (audioEngine.getAppProperties().getUserSettings()
                                 ->getValue ("recentPMixDocumentFiles"));

  recentFiles.addFile (file);

  audioEngine.getAppProperties().getUserSettings()
  ->setValue ("recentPMixDocumentFiles", recentFiles.toString());
}

static XmlElement* createNodeXml (AudioProcessorGraph::Node* const node) noexcept
{
  AudioPluginInstance* plugin = dynamic_cast <AudioPluginInstance*> (node->getProcessor());

  if (plugin == nullptr)
  {
    jassertfalse;
    return nullptr;
  }

  XmlElement* e = new XmlElement ("FILTER");
  e->setAttribute ("uid", (int) node->nodeId);
  e->setAttribute ("x", node->properties ["x"].toString());
  e->setAttribute ("y", node->properties ["y"].toString());
  e->setAttribute ("uiLastX", node->properties ["uiLastX"].toString());
  e->setAttribute ("uiLastY", node->properties ["uiLastY"].toString());

  PluginDescription pd;
  plugin->fillInPluginDescription (pd);

  e->addChildElement (pd.createXml());

  XmlElement* state = new XmlElement ("STATE");

  MemoryBlock m;
  node->getProcessor()->getStateInformation (m);
  state->addTextElement (m.toBase64Encoding());
  e->addChildElement (state);

  return e;
}

void PMixDocument::createNodeFromXml (const XmlElement& xml)
{
  PluginDescription pd;

  forEachXmlChildElement (xml, e)
  {
    if (pd.loadFromXml (*e))
      break;
  }

  String errorMessage;

  AudioPluginInstance* instance = audioEngine.getFormatManager().createPluginInstance (pd, audioEngine.getGraph().getSampleRate(), audioEngine.getGraph().getBlockSize(), errorMessage);

  if (instance == nullptr)
  {
    // xxx handle ins + outs
  }

  if (instance == nullptr)
    return;

  AudioProcessorGraph::Node::Ptr node (audioEngine.getGraph().addNode (instance, xml.getIntAttribute ("uid")));

  if (const XmlElement* const state = xml.getChildByName ("STATE"))
  {
    MemoryBlock m;
    m.fromBase64Encoding (state->getAllSubText());

    node->getProcessor()->setStateInformation (m.getData(), (int) m.getSize());
  }

  node->properties.set ("x", xml.getDoubleAttribute ("x"));
  node->properties.set ("y", xml.getDoubleAttribute ("y"));
  node->properties.set ("uiLastX", xml.getIntAttribute ("uiLastX"));
  node->properties.set ("uiLastY", xml.getIntAttribute ("uiLastY"));
}

XmlElement* PMixDocument::createXml() const
{
  XmlElement* xml = new XmlElement ("PMIXDOC");

  for (int i = 0; i < audioEngine.getGraph().getNumNodes(); ++i)
    xml->addChildElement (createNodeXml (audioEngine.getGraph().getNode (i)));

  for (int i = 0; i < audioEngine.getGraph().getNumConnections(); ++i)
  {
    const AudioProcessorGraph::Connection* const fc = audioEngine.getGraph().getConnection(i);

    XmlElement* e = new XmlElement ("CONNECTION");

    e->setAttribute ("srcFilter", (int) fc->sourceNodeId);
    e->setAttribute ("srcChannel", fc->sourceChannelIndex);
    e->setAttribute ("dstFilter", (int) fc->destNodeId);
    e->setAttribute ("dstChannel", fc->destChannelIndex);
    
    xml->addChildElement (e);
  }
  
//  XmlElement* e = new XmlElement ("MISC");
//
//  e->setAttribute ("snapPixels", snapGridPixels);
//  e->setAttribute ("snapActive", snapActive);
//  e->setAttribute ("snapShown", snapShown);
//  e->setAttribute ("overlayOpacity", String (componentOverlayOpacity, 3));
//  xml->addChildElement (e);

  return xml;
}

void PMixDocument::restoreFromXml (const XmlElement& xml)
{
  clear();

  forEachXmlChildElementWithTagName (xml, e, "FILTER")
  {
    createNodeFromXml (*e);
    changed();
  }

  // without this here, dynamic IO audio processors don't get set up and the connections fail
  audioEngine.getGraph().prepareToPlay(44100., 1024);

  forEachXmlChildElementWithTagName (xml, e, "CONNECTION")
  {
    addConnection ((uint32) e->getIntAttribute ("srcFilter"),
                   e->getIntAttribute ("srcChannel"),
                   (uint32) e->getIntAttribute ("dstFilter"),
                   e->getIntAttribute ("dstChannel"));
  }

  audioEngine.getGraph().removeIllegalConnections();
}

bool PMixDocument::isSnapActive (const bool disableIfCtrlKeyDown) const noexcept
{
  return snapActive != (disableIfCtrlKeyDown && ModifierKeys::getCurrentModifiers().isCtrlDown());
}

int PMixDocument::snapPosition (int pos) const noexcept
{
  if (isSnapActive (true))
  {
    jassert (snapGridPixels > 0);
    pos = ((pos + snapGridPixels * 1024 + snapGridPixels / 2) / snapGridPixels - 1024) * snapGridPixels;
  }
  
  return pos;
}

void PMixDocument::setSnappingGrid (const int numPixels, const bool active, const bool shown)
{
  if (numPixels != snapGridPixels
      || active != snapActive
      || shown != snapShown)
  {
    snapGridPixels = numPixels;
    snapActive = active;
    snapShown = shown;
    changed();
  }
}

void PMixDocument::setComponentOverlayOpacity (const float alpha)
{
  if (alpha != componentOverlayOpacity)
  {
    componentOverlayOpacity = alpha;
    changed();
  }
}

void PMixDocument::beginTransaction()
{
  getUndoManager().beginNewTransaction();
}

void PMixDocument::beginTransaction (const String& name)
{
  getUndoManager().beginNewTransaction (name);
}

bool PMixDocument::perform (UndoableAction* const action, const String& actionName)
{
  return undoManager.perform (action, actionName);
}

void PMixDocument::initialize()
{
  InternalPluginFormat internalFormat;
  
  addFilter (internalFormat.getDescriptionFor (InternalPluginFormat::audioInputFilter),  0.5f,  0.1f);
  addFilter (internalFormat.getDescriptionFor (InternalPluginFormat::midiInputFilter),   0.25f, 0.1f);
  addFilter (internalFormat.getDescriptionFor (InternalPluginFormat::audioOutputFilter), 0.5f,  0.9f);
  
  setChangedFlag (false);
}

