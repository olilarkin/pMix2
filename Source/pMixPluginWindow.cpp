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
  //setDropShadowEnabled(false);
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
    if (type == Generic || type == Parameters)
      ui = new PMixGenericAudioProcessorEditor (processor);
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
  delete this;
}

//ProcessorProgramPropertyComp::ProcessorProgramPropertyComp (const String& name, AudioProcessor& p, int index_)
//: PropertyComponent (name),
//owner (p),
//index (index_)
//{
//  owner.addListener (this);
//}
//
//ProcessorProgramPropertyComp::~ProcessorProgramPropertyComp()
//{
//  owner.removeListener (this);
//}
//
//void ProcessorProgramPropertyComp::refresh() { }
//void ProcessorProgramPropertyComp::audioProcessorChanged (AudioProcessor*) { }
//void ProcessorProgramPropertyComp::audioProcessorParameterChanged (AudioProcessor*, int, float) { }
//
//#pragma mark -
//#pragma mark ProgramAudioProcessorEditor
//
//ProgramAudioProcessorEditor::ProgramAudioProcessorEditor (AudioProcessor* const p)
//: AudioProcessorEditor (p)
//{
//  jassert (p != nullptr);
//  setOpaque (true);
//  
//  addAndMakeVisible (&panel);
//  
//  Array<PropertyComponent*> programs;
//  
//  const int numPrograms = p->getNumPrograms();
//  int totalHeight = 0;
//  
//  for (int i = 0; i < numPrograms; ++i)
//  {
//    String name (p->getProgramName (i).trim());
//    
//    if (name.isEmpty())
//      name = "Unnamed";
//    
//    ProcessorProgramPropertyComp* const pc = new ProcessorProgramPropertyComp (name, *p, i);
//    programs.add (pc);
//    totalHeight += pc->getPreferredHeight();
//  }
//  
//  panel.addProperties (programs);
//  
//  setSize (400, jlimit (25, 400, totalHeight));
//}
//
//void ProgramAudioProcessorEditor::paint (Graphics& g)
//{
//  g.fillAll (Colours::grey);
//}
//
//void ProgramAudioProcessorEditor::resized()
//{
//  panel.setBounds (getLocalBounds());
//}