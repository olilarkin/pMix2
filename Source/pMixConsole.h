/*
  ==============================================================================

    pMixConsole.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#ifndef PMIXCONSOLE_H_INCLUDED
#define PMIXCONSOLE_H_INCLUDED

#include "JuceHeader.h"

class ConsoleListBoxModel   : public ListBoxModel
{
public:
    ConsoleListBoxModel (const Array<String>& list)
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

            g.drawText (messageList.getFirst(),
                        Rectangle<int> (width, height).reduced (4, 0),
                        Justification::centredLeft, true);
        }
    }

private:
    const Array<String>& messageList;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConsoleListBoxModel)
};

class Console  : public Component
               , private AsyncUpdater
{
public:
  Console();
  ~Console();
  
  void paint (Graphics& g) override;
  
  void resized() override;

private:
  ListBox messageListBox;
  Array<String> messageList;
  ConsoleListBoxModel listBoxModel;


  // This is used to dispach an incoming message to the message thread
  struct IncomingMessageCallback   : public CallbackMessage
  {
    IncomingMessageCallback (Console* d, const String& m)
    : console (d), message (m) {}
    
    void messageCallback() override
    {
      if (console != nullptr)
        console->addMessageToList (message);
    }

    Component::SafePointer<Console> console;
    String message;
  };

  void postMessageToList (const String& message);
  void addMessageToList (const String& message);

  void handleAsyncUpdate() override;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Console);
};


#endif  // PMIXCONSOLE_H_INCLUDED
