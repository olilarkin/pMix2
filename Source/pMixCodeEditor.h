/*
  ==============================================================================

    CodeEditor.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#ifndef CODEEDITOR_H_INCLUDED
#define CODEEDITOR_H_INCLUDED

#include "JuceHeader.h"
#include "pMixAudioEngine.h"
#include "FaustAudioProcessor.h"

class CodeEditor : public Component
                 , public ChangeListener
                 , public Button::Listener
{
public:
  CodeEditor(PMixAudioEngine& audioEngine);
  
  void paint (Graphics& g) override;
  
  void resized() override;
  
  void changeListenerCallback (ChangeBroadcaster* source);
  void buttonClicked (Button* button);
  
private:
  CPlusPlusCodeTokeniser cppTokeniser;
  CodeDocument codeDocument;
  ScopedPointer<CodeEditorComponent> editor;
  PMixAudioEngine& audioEngine;
  TextButton compileButton;
  TextButton svgButton;
  FaustAudioProcessor* selectedFaustAudioProcessor;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CodeEditor);
};



#endif  // CODEEDITOR_H_INCLUDED
