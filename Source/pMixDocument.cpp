#include "pMixDocument.h"
#include "pMixAudioEngine.h"
#include "pMixConstants.h"

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
  startTimer (20);
}

PMixDocument::~PMixDocument()
{
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
    
    AudioPluginInstance* instance;
    
    if (desc->pluginFormatName == "FAUST") {
      instance = audioEngine.getFaustFormat().createInstanceFromDescription(*desc, audioEngine.getGraph().getSampleRate(), audioEngine.getGraph().getBlockSize());
    }
    else
    {
      instance = audioEngine.getFormatManager().createPluginInstance (*desc, audioEngine.getGraph().getSampleRate(), audioEngine.getGraph().getBlockSize(), errorMessage);
    }
    
    if (instance != nullptr)
      node = audioEngine.getGraph().addNode (instance);

    FaustAudioPluginInstance* faustProc = dynamic_cast<FaustAudioPluginInstance*>(instance);
    
    if (faustProc)
    {
      const File drawPath = File(FAUST_DRAW_PATH);
      faustProc->initialize(getLibraryPath(), drawPath);
    }

    if (node != nullptr)
    {
      node->properties.set ("x", x);
      node->properties.set ("y", y);
      node->properties.set ("uiLastX", Random::getSystemRandom().nextInt (500));
      node->properties.set ("uiLastY", Random::getSystemRandom().nextInt (500));
      
      if (!InternalPluginFormat::isInternalFormat(desc->name))
      {
        node->properties.set ("colour", defaultColours.getNextColour().toString());
        node->properties.set ("iposx", x);
        node->properties.set ("iposy", y);
        node->properties.set ("update", false);
        Array<var> presets;
        node->properties.set("presets", presets);
        Array<var> params;
        node->properties.set("params", params);
      }

      changed();
    }
    else
    {
      AlertWindow::showMessageBox (AlertWindow::WarningIcon,
                                   TRANS("Couldn't create node"),
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

void PMixDocument::setNodePosition (const uint32 nodeId, double x, double y)
{
  const AudioProcessorGraph::Node::Ptr n (audioEngine.getGraph().getNodeForId (nodeId));

  if (n != nullptr)
  {
    n->properties.set ("x", jlimit (0.0, 1.0, x));
    n->properties.set ("y", jlimit (0.0, 1.0, y));
  }
}

void PMixDocument::getNodePosition (const uint32 nodeId, double& x, double& y) const
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

 XmlElement* PMixDocument::createNodeXml (AudioProcessorGraph::Node* const node) noexcept
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
  
  if(!InternalPluginFormat::isInternalFormat(pd.name))
  {
    e->setAttribute("colour", node->properties ["colour"].toString());
    e->setAttribute ("iposx", node->properties ["iposx"].toString());
    e->setAttribute ("iposy", node->properties ["iposy"].toString());
  }
  
  e->addChildElement (pd.createXml());

  XmlElement* state = new XmlElement ("STATE");

  MemoryBlock m;
  node->getProcessor()->getStateInformation (m);
  state->addTextElement (m.toBase64Encoding());
  e->addChildElement (state);
  
  if(!InternalPluginFormat::isInternalFormat(pd.name))
  {
    XmlElement* params = new XmlElement ("PARAMS");
    params->addTextElement (JSON::toString(node->properties ["params"], false));
    e->addChildElement(params);
    
    XmlElement* presets = new XmlElement ("PRESETS");
    presets->addTextElement (JSON::toString(node->properties ["presets"], false));
    e->addChildElement(presets);
  }
  
  return e;
}

//TODO: why two createNodeFromXml methods
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
  
  if (pd.pluginFormatName == "FAUST")
  {
    FaustAudioPluginInstance* faustProc = dynamic_cast<FaustAudioPluginInstance*>(instance);
    faustProc->initialize(getLibraryPath());
  }

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
  
  if(!InternalPluginFormat::isInternalFormat(pd.name))
  {
    node->properties.set ("colour", xml.getStringAttribute ("colour"));
    node->properties.set ("iposx", xml.getDoubleAttribute ("iposx"));
    node->properties.set ("iposy", xml.getDoubleAttribute ("iposy"));
    
    if (const XmlElement* const params = xml.getChildByName ("PARAMS"))
    {
      var vparams = JSON::parse(params->getAllSubText());
      node->properties.set ("params", vparams);
    }
    
    if (const XmlElement* const presets = xml.getChildByName ("PRESETS"))
    {
      var vpresets = JSON::parse(presets->getAllSubText());
      node->properties.set ("presets", vpresets);
    }
  }
}

void PMixDocument::createFaustNodeFromXml (XmlElement& xml, const String& newSourceCode)
{
  PluginDescription pd;
  
  forEachXmlChildElement (xml, e)
  {
    if (pd.loadFromXml (*e))
      break;
  }
  
  String errorMessage;
  
  AudioPluginInstance* instance = audioEngine.getFormatManager().createPluginInstance (pd, audioEngine.getGraph().getSampleRate(), audioEngine.getGraph().getBlockSize(), errorMessage);
  
  FaustAudioPluginInstance* faustProc = dynamic_cast<FaustAudioPluginInstance*>(instance);
  faustProc->initialize(getLibraryPath());
  
  if (newSourceCode.length())
    faustProc->setSourceCode(newSourceCode, true);
  
  // TODO: this is a bit wrong!
  faustProc->prepareToPlay(44100., 8192);
  
  xml.setAttribute("numInputs", faustProc->getNumInputChannels());
  xml.setAttribute("numOutputs", faustProc->getNumOutputChannels());
  
  AudioProcessorGraph::Node::Ptr node (audioEngine.getGraph().addNode (instance, xml.getIntAttribute ("uid")));
  
  //TODO: reload state if compile failed
//  if (const XmlElement* const state = xml.getChildByName ("STATE"))
//  {
//    MemoryBlock m;
//    m.fromBase64Encoding (state->getAllSubText());
//    
//    node->getProcessor()->setStateInformation (m.getData(), (int) m.getSize());
//  }
  
  node->properties.set ("x", xml.getDoubleAttribute ("x"));
  node->properties.set ("y", xml.getDoubleAttribute ("y"));
  node->properties.set ("uiLastX", xml.getIntAttribute ("uiLastX"));
  node->properties.set ("uiLastY", xml.getIntAttribute ("uiLastY"));

  node->properties.set ("colour", xml.getStringAttribute ("colour"));
  node->properties.set ("iposx", xml.getDoubleAttribute ("iposx"));
  node->properties.set ("iposy", xml.getDoubleAttribute ("iposy"));
  
  if (const XmlElement* const params = xml.getChildByName ("PARAMS"))
  {
    var vparams = JSON::parse(params->getAllSubText());
    node->properties.set ("params", vparams);
  }
  
  if (const XmlElement* const presets = xml.getChildByName ("PRESETS"))
  {
    var vpresets = JSON::parse(presets->getAllSubText());
    node->properties.set ("presets", vpresets);
  }
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
  //addFilter (internalFormat.getDescriptionFor (InternalPluginFormat::midiInputFilter),   0.25f, 0.1f);
  addFilter (internalFormat.getDescriptionFor (InternalPluginFormat::audioOutputFilter), 0.5f,  0.9f);
  
  setChangedFlag (false);
}

String PMixDocument::getLibraryPath()
{
  String fullLibraryPath;

#if JUCE_MAC
#if PMIX_PLUGIN
  CFBundleRef bundle = CFBundleGetBundleWithIdentifier(CFSTR("com.OliLarkin.pMixPlugin"));
#else
  // OSX only : access to the pMix bundle
  CFBundleRef bundle = CFBundleGetBundleWithIdentifier(CFSTR("com.OliLarkin.pMix"));
#endif
  CFURLRef ref = CFBundleCopyBundleURL(bundle);
  UInt8 bundlePath[512];
  Boolean res = CFURLGetFileSystemRepresentation(ref, true, bundlePath, 512);
  jassert(res);
  fullLibraryPath << (const char*) bundlePath << FAUST_LIBRARY_PATH;
#endif //JUCE_MAC
  
  return fullLibraryPath;
}

void PMixDocument::addPreset(const uint32 nodeId, double x, double y)
{
  AudioProcessorGraph::Node::Ptr node = getNodeForId(nodeId);
  AudioPluginInstance* plugin = dynamic_cast <AudioPluginInstance*> (node->getProcessor());

  var* presetsArr = node->properties.getVarPointer("presets");
  
  Array<var> paramValues;
  
  for(int p=0; p<plugin->getNumParameters(); p++)
  {
    paramValues.add(plugin->getParameter(p));
  }

  String name;
  name << "Preset " << presetsArr->getArray()->size() + 1;
  DynamicObject* obj = new DynamicObject();
  obj->setProperty("name", name);
  obj->setProperty("x", x);
  obj->setProperty("y", y);
  obj->setProperty("radius", 1.);
  obj->setProperty("hidden", false);
//  obj->setProperty("distance", 0.);
  obj->setProperty("coeff", 1.);
  obj->setProperty("state", paramValues);

  var preset = var(obj);
  presetsArr->append(preset);

  setFilterIPos(nodeId, x, y); // will call changed()
}

void PMixDocument::removePreset(const uint32 nodeId, const int presetIdx)
{
  const AudioProcessorGraph::Node::Ptr node (audioEngine.getGraph().getNodeForId (nodeId));
  
  if (node != nullptr)
  {
    Array<var>* presetsArr = node->properties.getVarPointer("presets")->getArray();
    presetsArr->remove(presetIdx);
  }
  
  changed();
}

void PMixDocument::setPresetPosition (const uint32 nodeId, const int presetIdx, double x, double y)
{
  const AudioProcessorGraph::Node::Ptr node (audioEngine.getGraph().getNodeForId (nodeId));
  
  if (node != nullptr)
  {
    Array<var>* presetsArr = node->properties.getVarPointer("presets")->getArray();
    DynamicObject* obj = presetsArr->getReference(presetIdx).getDynamicObject();
    obj->setProperty("x", jlimit (0.0, 1.0, x));
    obj->setProperty("y", jlimit (0.0, 1.0, y));
  }
  
  changed();
}

void PMixDocument::getPresetPosition (const uint32 nodeId, const int presetIdx, double& x, double& y) const
{
  x = y = 0;
  const AudioProcessorGraph::Node::Ptr node (audioEngine.getGraph().getNodeForId (nodeId));
  
  if (node != nullptr)
  {
    Array<var>* presetsArr = node->properties.getVarPointer("presets")->getArray();
    DynamicObject* obj = presetsArr->getReference(presetIdx).getDynamicObject();
    x = (double) obj->getProperty("x");
    y = (double) obj->getProperty("y");
  }
}

double PMixDocument::getPresetWeight(const uint32 nodeId, const int presetIdx)
{
  const AudioProcessorGraph::Node::Ptr node (audioEngine.getGraph().getNodeForId (nodeId));
  
  if (node != nullptr)
  {
    Array<var>* presetsArr = node->properties.getVarPointer("presets")->getArray();
    
    if (presetIdx < presetsArr->size())
    {
      DynamicObject* obj = presetsArr->getReference(presetIdx).getDynamicObject();
      return (double) obj->getProperty("coeff");
    }
  }
  
  return 0.;
}

void PMixDocument::setPresetName(const uint32 nodeId, const int presetIdx, String newName)
{
  const AudioProcessorGraph::Node::Ptr node (audioEngine.getGraph().getNodeForId (nodeId));
  
  if (node != nullptr)
  {
    Array<var>* presetsArr = node->properties.getVarPointer("presets")->getArray();
    DynamicObject* obj = presetsArr->getReference(presetIdx).getDynamicObject();
    obj->setProperty("name", newName);
  }
  
  changed();
}

void PMixDocument::setFilterColour(const uint32 nodeId, const Colour colour)
{
  const AudioProcessorGraph::Node::Ptr node (audioEngine.getGraph().getNodeForId (nodeId));

  if (node != nullptr)
  {
    node->properties.set("colour", colour.toString());
    changed();
  }
}

Colour PMixDocument::getFilterColour(const uint32 nodeId) const
{
  const AudioProcessorGraph::Node::Ptr node (audioEngine.getGraph().getNodeForId (nodeId));

  Colour clr = Colours::red;
  
  if (node != nullptr)
  {
    clr = Colour::fromString(node->properties ["colour"].toString());
  }
  
  return clr;
}

void PMixDocument::setParameterToInterpolate(const uint32 nodeId, const int paramIdx, bool interpolate)
{
  const AudioProcessorGraph::Node::Ptr node (audioEngine.getGraph().getNodeForId (nodeId));
  
  if (node != nullptr)
  {
    Array<var>* params = node->properties.getVarPointer("params")->getArray();
    
    if (interpolate)
    {
      params->addIfNotAlreadyThere(paramIdx);
    }
    else
    {
      params->removeAllInstancesOf(paramIdx);
    }
  }
}

bool PMixDocument::getParameterIsInterpolated(const uint32 nodeId, const int paramIdx)
{
  const AudioProcessorGraph::Node::Ptr node (audioEngine.getGraph().getNodeForId (nodeId));
  
  if (node != nullptr)
  {
    Array<var>* params = node->properties.getVarPointer("params")->getArray();
    
    return params->contains(paramIdx);
  }
  
  return false;
}

void PMixDocument::setFilterIPos(const uint32 nodeId, double x, double y)
{
  const AudioProcessorGraph::Node::Ptr node (audioEngine.getGraph().getNodeForId (nodeId));
  
  if (node != nullptr)
  {
    double iposx = x ;
    double iposy = y;
    
    node->properties.set("iposx", x);
    node->properties.set("iposy", y);

    Array<var>* presetsArr = node->properties.getVarPointer("presets")->getArray();
    int numPresets = presetsArr->size();
    Array<double> distances;
    
    // work out the disstances
    for (int presetIdx = 0; presetIdx < numPresets; presetIdx++)
    {
      DynamicObject* obj = presetsArr->getReference(presetIdx).getDynamicObject();
      double pposx = (double) obj->getProperty("x");
      double pposy = (double) obj->getProperty("y");
      
      double a = pposx - iposx;    //x dist
      double b = pposy - iposy;    //y dist
      
      if(a == 0. && b == 0.)
        distances.add(0.);
      else
        distances.add(sqrt(a*a+b*b));
    }
    
    // now do coeefficient
    
    double sumf = 0.; // sum of functions
    
    for (int presetIdx = 0; presetIdx < numPresets; presetIdx++)
    {
      DynamicObject* obj = presetsArr->getReference(presetIdx).getDynamicObject();
      bool hidden = (bool) obj->getProperty("hidden");
      double radius = (double) obj->getProperty("radius");
      
      double square = distances[presetIdx] * distances[presetIdx];
      
      if(!hidden)
      {
        sumf += (radius / square);
      }
    }
    
    for(int presetIdx = 0; presetIdx < numPresets; presetIdx++)
    {
      DynamicObject* obj = presetsArr->getReference(presetIdx).getDynamicObject();
      bool hidden = (bool) obj->getProperty("hidden");
      double radius = (double) obj->getProperty("radius");
      
      double coeff = 0.;
      
      if(distances[presetIdx] == 0.)
        coeff = 1.;
      else
      {
        double square = distances[presetIdx] * distances[presetIdx];
        
        if(!hidden)
        {
          coeff = (radius / square) / sumf;
        }
      }
      
      obj->setProperty("coeff", coeff);
    }
    
    changed();
    
    node->properties.set("update", true);
  }
}

void PMixDocument::getFilterIPos(const uint32 nodeId, double& x, double& y) const
{
  const AudioProcessorGraph::Node::Ptr node (audioEngine.getGraph().getNodeForId (nodeId));
  
  if (node != nullptr)
  {
    x = (double) node->properties["iposx"];
    y = (double) node->properties["iposy"];
  }
}

void PMixDocument::timerCallback()
{
  for (int i = getNumFilters(); --i >= 0;)
  {
    const AudioProcessorGraph::Node::Ptr node (audioEngine.getDoc().getNode (i));
    
    if (!InternalPluginFormat::isInternalFormat(node->getProcessor()->getName()))
    {
      bool needsUpdate = node->properties["update"];
      
      if (needsUpdate)
      {
        Array<var>* params = node->properties.getVarPointer("params")->getArray();
        Array<var>* presetsArr = node->properties.getVarPointer("presets")->getArray();

        //loop over all parameters marked for interpolating
        for (int item=0; item<params->size(); item++)
        {
          var parameterIdx = params->getReference(item);
          
          double sum = 0.;
          
          //loop over all presets
          for (int presetIdx=0; presetIdx < presetsArr->size(); presetIdx++)
          {
            DynamicObject* obj = presetsArr->getReference(presetIdx).getDynamicObject();
            double coeff = obj->getProperty("coeff");
            Array<var>* preset = obj->getProperty("state").getArray();
            sum = sum + (double(preset->getReference(parameterIdx)) * coeff);
          }
          
          node->getProcessor()->setParameterNotifyingHost(parameterIdx, (float) sum);
        }
        
        node->properties.set("update", false);
      }
    }
  }
}

