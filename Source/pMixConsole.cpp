/*
  ==============================================================================

    pMixConsole.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixConsole.h"
#include "pMixLogger.h"

PMixConsole::PMixConsole()
: listBoxModel (messageList)
{
  setOpaque (true);
  
  addAndMakeVisible (messageListBox);
  messageListBox.setModel (&listBoxModel);
  messageListBox.setColour (ListBox::backgroundColourId, Colour (0x32ffffff));
  messageListBox.setColour (ListBox::outlineColourId, Colours::black);
}

PMixConsole::~PMixConsole()
{
}

void PMixConsole::paint (Graphics& g)
{
  g.fillAll (Colours::white);
}

void PMixConsole::resized()
{
  Rectangle<int> area (getLocalBounds());
  messageListBox.setBounds (area.reduced (8));
}

void PMixConsole::postMessageToList (const String& message)
{
  (new IncomingMessageCallback (this, message))->post();
}

void PMixConsole::addMessageToList (const String& message)
{
  messageList.add (message);
  triggerAsyncUpdate();
}

void PMixConsole::handleAsyncUpdate()
{
  messageListBox.updateContent();
  messageListBox.scrollToEnsureRowIsOnscreen (messageList.size() - 1);
  messageListBox.repaint();
}

void PMixConsole::changeListenerCallback (ChangeBroadcaster* source)
{
  PMixLogger* logger = dynamic_cast<PMixLogger*>(source);
  
  if (logger)
  {
    String message;
    while (logger->getLastMessage(message))
    {
      postMessageToList(message);
    }
  }
}

void PMixConsole::clear()
{
  messageList.clear();
  messageListBox.updateContent();
  messageListBox.scrollToEnsureRowIsOnscreen (messageList.size() - 1);
  messageListBox.repaint();
}