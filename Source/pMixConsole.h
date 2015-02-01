/*
  ==============================================================================

    pMixConsole.h
    Created: 1 Feb 2015 9:42:31pm
    Author:  Oliver Larkin

  ==============================================================================
*/

#ifndef PMIXCONSOLE_H_INCLUDED
#define PMIXCONSOLE_H_INCLUDED

#include "JuceHeader.h"

class pMixConsoleListBoxModel   : public ListBoxModel
{
public:
    pMixConsoleListBoxModel (const Array<String>& list)
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

            double time = 0;

            g.drawText (String::formatted ("%02d:%02d:%02d",
                                           ((int) (time / 3600.0)) % 24,
                                           ((int) (time / 60.0)) % 60,
                                           ((int) time) % 60)
                            + "  -  Hello World",
                        Rectangle<int> (width, height).reduced (4, 0),
                        Justification::centredLeft, true);
        }
    }

private:
    const Array<String>& messageList;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pMixConsoleListBoxModel)
};




#endif  // PMIXCONSOLE_H_INCLUDED
