/*
  ==============================================================================

    FaustCodeTokenizer.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#ifndef FAUSTCODETOKENIZER_H_INCLUDED
#define FAUSTCODETOKENIZER_H_INCLUDED

#include "JuceHeader.h"

class FaustTokeniser   : public CodeTokeniser
{
public:
  //==============================================================================
  FaustTokeniser();
  ~FaustTokeniser();
  
  //==============================================================================
  int readNextToken (CodeDocument::Iterator&) override;
  CodeEditorComponent::ColourScheme getDefaultColourScheme() override;
  
  /** The token values returned by this tokeniser. */
  enum TokenType
  {
    tokenType_error = 0,
    tokenType_comment,
    tokenType_primitive,
    tokenType_operator,
    tokenType_identifier,
    tokenType_integer,
    tokenType_float,
    tokenType_string,
    tokenType_bracket,
    tokenType_punctuation
  };
  
private:
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FaustTokeniser)
};


#endif  // FAUSTCODETOKENIZER_H_INCLUDED
