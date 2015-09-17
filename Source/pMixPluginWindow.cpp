/*
  ==============================================================================

    pMixPluginWindow.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixPluginWindow.h"
#include "pMixGenericEditor.h"

class PluginWindow;
static Array <PluginWindow*> activePluginWindows;

PluginWindow::PluginWindow (Component* const pluginEditor, AudioProcessorGraph::Node* const o, WindowFormatType t)
: DocumentWindow (pluginEditor->getName(), Colours::lightblue, DocumentWindow::minimiseButton | DocumentWindow::closeButton),
owner (o),
type (t)
{
  setSize (400, 300);
  setUsingNativeTitleBar(true);
  setAlwaysOnTop(true);
  setContentOwned (pluginEditor, true);

  setTopLeftPosition (owner->properties.getWithDefault ("uiLastX", Random::getSystemRandom().nextInt (500)),
                      owner->properties.getWithDefault ("uiLastY", Random::getSystemRandom().nextInt (500)));
  setVisible (true);
  activePluginWindows.add (this);
}

void PluginWindow::closeCurrentlyOpenWindowsFor (const uint32 nodeId)
{
  for (int i = activePluginWindows.size(); --i >= 0;)
    if (activePluginWindows.getUnchecked(i)->owner->nodeId == nodeId)
      delete activePluginWindows.getUnchecked (i);
}

void PluginWindow::closeAllCurrentlyOpenWindows()
{
  if (activePluginWindows.size() > 0)
  {
    for (int i = activePluginWindows.size(); --i >= 0;)
      delete activePluginWindows.getUnchecked (i);
    
    Component dummyModalComp;
    dummyModalComp.enterModalState();
    MessageManager::getInstance()->runDispatchLoopUntil (50);
  }
}

PluginWindow* PluginWindow::getWindowFor (AudioProcessorGraph::Node* const node,
                                          WindowFormatType type)
{
  jassert (node != nullptr);
  
  for (int i = activePluginWindows.size(); --i >= 0;)
    if (activePluginWindows.getUnchecked(i)->owner == node
        && activePluginWindows.getUnchecked(i)->type == type)
      return activePluginWindows.getUnchecked(i);
  
  AudioProcessor* processor = node->getProcessor();
  AudioProcessorEditor* ui = nullptr;
  
  if (type == Normal)
  {
    ui = processor->createEditorIfNeeded();
    
    if (ui == nullptr)
      type = Generic;
  }
  
  if (ui == nullptr)
  {
    //if (type == Generic || type == Parameters)
      //ui = new PMixGenericAudioProcessorEditor (processor);
//    else if (type == Programs)
//      ui = new ProgramAudioProcessorEditor (processor);
  }
  
  if (ui != nullptr)
  {
    if (AudioPluginInstance* const plugin = dynamic_cast<AudioPluginInstance*> (processor))
      ui->setName (plugin->getName());
    
    return new PluginWindow (ui, node, type);
  }
  
  return nullptr;
}

PluginWindow::~PluginWindow()
{
  dynamic_cast<AudioPluginInstance*> (owner->getProcessor())->editorBeingDeleted( dynamic_cast<AudioProcessorEditor*>(getContentComponent()) );
  activePluginWindows.removeFirstMatchingValue (this);
  clearContentComponent();
}

void PluginWindow::moved()
{
  owner->properties.set ("uiLastX", getX());
  owner->properties.set ("uiLastY", getY());
}

void PluginWindow::closeButtonPressed()
{
  dynamic_cast<AudioPluginInstance*> (owner->getProcessor())->editorBeingDeleted( dynamic_cast<AudioProcessorEditor*>(getContentComponent()) );
  delete this;
}