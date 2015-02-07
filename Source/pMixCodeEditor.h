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

class CodeEditor : public Component
                 , public ChangeListener
{
public:
  CodeEditor(PMixAudioEngine& audioEngine);
  
  void paint (Graphics& g) override;
  
  void resized() override;
  
  void changeListenerCallback (ChangeBroadcaster* source);

private:
  CPlusPlusCodeTokeniser cppTokeniser;
  CodeDocument codeDocument;
  ScopedPointer<CodeEditorComponent> editor;
  PMixAudioEngine& audioEngine;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CodeEditor);
};



#endif  // CODEEDITOR_H_INCLUDED
