/*
  ==============================================================================

    CodeEditor.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#ifndef CODEEDITOR_H_INCLUDED
#define CODEEDITOR_H_INCLUDED

#include "JuceHeader.h"

class CodeEditor : public Component
{
public:
  CodeEditor();
  
  void paint (Graphics& g) override;
  
  void resized() override;
  
private:
  CPlusPlusCodeTokeniser cppTokeniser;
  CodeDocument codeDocument;
  ScopedPointer<CodeEditorComponent> editor;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CodeEditor);
};



#endif  // CODEEDITOR_H_INCLUDED
