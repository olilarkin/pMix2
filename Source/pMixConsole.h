/*
  ==============================================================================

    pMixConsole.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class PMixConsoleListBoxModel   : public ListBoxModel
{
public:
    PMixConsoleListBoxModel (const Array<String>& list)
        : messageList (list)
    {
    }

    int getNumRows() override    { return messageList.size(); }

    void paintListBoxItem (int row, Graphics& g, int width, int height, bool rowIsSelected) override
    {
        if (rowIsSelected)
            g.fillAll (Colours::blue.withAlpha (0.2f));

        if (isPositiveAndBelow (row, messageList.size()))
        {
            g.setColour (Colours::black);
            const String& message = messageList.getReference (row);

            g.drawText (message,
                        Rectangle<int> (width, height).reduced (4, 0),
                        Justification::centredLeft, true);
        }
    }

private:
    const Array<String>& messageList;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PMixConsoleListBoxModel)
};

class PMixConsole  : public Component
               , private AsyncUpdater
               , public ChangeListener
{
public:
  PMixConsole();
  ~PMixConsole();
  
  void paint (Graphics& g) override;
  
  void resized() override;

  void changeListenerCallback (ChangeBroadcaster* source) override;

  void clear();
  
private:
  ListBox messageListBox;
  Array<String> messageList;
  PMixConsoleListBoxModel listBoxModel;


  // This is used to dispach an incoming message to the message thread
  struct IncomingMessageCallback   : public CallbackMessage
  {
    IncomingMessageCallback (PMixConsole* d, const String& m)
    : PMixConsole (d), message (m) {}
    
    void messageCallback() override
    {
      if (PMixConsole != nullptr)
        PMixConsole->addMessageToList (message);
    }

    Component::SafePointer<PMixConsole> PMixConsole;
    String message;
  };

  void postMessageToList (const String& message);
  void addMessageToList (const String& message);

  void handleAsyncUpdate() override;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PMixConsole);
};


