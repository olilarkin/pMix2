/*
  ==============================================================================

    pMixConsole.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixConsole.h"

Console::Console()
: listBoxModel (messageList)
{
  setOpaque (true);
  
  addAndMakeVisible (messageListBox);
  messageListBox.setModel (&listBoxModel);
  messageListBox.setColour (ListBox::backgroundColourId, Colour (0x32ffffff));
  messageListBox.setColour (ListBox::outlineColourId, Colours::black);
  
  addMessageToList("Hello");
}

Console::~Console()
{
}

void Console::paint (Graphics& g)
{
  g.fillAll (Colours::white);
}

void Console::resized()
{
  Rectangle<int> area (getLocalBounds());
  messageListBox.setBounds (area.reduced (8));
}

void Console::postMessageToList (const String& message)
{
  (new IncomingMessageCallback (this, message))->post();
}

void Console::addMessageToList (const String& message)
{
  messageList.add (message);
  triggerAsyncUpdate();
}

void Console::handleAsyncUpdate()
{
  messageListBox.updateContent();
  messageListBox.scrollToEnsureRowIsOnscreen (messageList.size() - 1);
  messageListBox.repaint();
}