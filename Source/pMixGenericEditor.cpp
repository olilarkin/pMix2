/*
  ==============================================================================

    pMixGenericEditor.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixGenericEditor.h"


PMixParamSlider::PMixParamSlider (AudioProcessor& p, int paramIndex)
: owner (p)
, index (paramIndex)
{
  const int steps = owner.getParameterNumSteps (index);
  
  if (steps > 1 && steps < 0x7fffffff)
    setRange (0.0, 1.0, 1.0 / (steps - 1.0));
  else
    setRange (0.0, 1.0);
  
  setSliderStyle (Slider::LinearHorizontal);
  setTextBoxStyle (Slider::NoTextBox, false, 0, 0);
  //setTextBoxIsEditable (true);
  setPopupMenuEnabled(true);
  setScrollWheelEnabled (true);
}

void PMixParamSlider::valueChanged()
{
  const float newVal = (float) getValue();
  
  if (owner.getParameter (index) != newVal)
  {
    owner.setParameterNotifyingHost (index, newVal);
    updateText();
  }
}
  
String PMixParamSlider::getTextFromValue (double value)
{
  return owner.getParameterText (index) + " " + owner.getParameterLabel (index).trimEnd();
}

PMixProcessorParameterPropertyComp::PMixProcessorParameterPropertyComp (const String& name, AudioProcessor& p, int paramIndex)
: PropertyComponent (name)
, owner (p)
, index (paramIndex)
, paramHasChanged (false)
, slider (p, paramIndex)
{
  startTimer (100);
  setPreferredHeight(16);
  //slider.setPopupDisplayEnabled(true, 0);
  addAndMakeVisible (slider);
  owner.addListener (this);
}
  
PMixProcessorParameterPropertyComp::~PMixProcessorParameterPropertyComp()
{
  owner.removeListener (this);
}
  
void PMixProcessorParameterPropertyComp::refresh()
{
  paramHasChanged = false;
  
  if (slider.getThumbBeingDragged() < 0)
    slider.setValue (owner.getParameter (index), dontSendNotification);
  
  slider.updateText();
}

void PMixProcessorParameterPropertyComp::audioProcessorParameterChanged (AudioProcessor*, int parameterIndex, float)
{
  if (parameterIndex == index)
    paramHasChanged = true;
}

void PMixProcessorParameterPropertyComp::timerCallback()
{
  if (paramHasChanged)
  {
    refresh();
    startTimerHz (50);
  }
  else
  {
    startTimer (jmin (1000 / 4, getTimerInterval() + 10));
  }
}

PMixGenericAudioProcessorEditor::PMixGenericAudioProcessorEditor (AudioProcessor* const p)
: AudioProcessorEditor (p)
{
  jassert (p != nullptr);
  //setOpaque (true);
  
  addAndMakeVisible (panel);
  
  Array <PropertyComponent*> params;
  
  const int numParams = p->getNumParameters();
  int totalHeight = 0;
  
  for (int i = 0; i < numParams; ++i)
  {
    String name (p->getParameterName (i));
    if (name.trim().isEmpty())
      name = "Unnamed";
    
    PMixProcessorParameterPropertyComp* const pc = new PMixProcessorParameterPropertyComp (name, *p, i);
    params.add (pc);
    totalHeight += pc->getPreferredHeight();
  }
  
  panel.addProperties (params);
  
  setSize (200, jlimit (25, 400, totalHeight));
}

PMixGenericAudioProcessorEditor::~PMixGenericAudioProcessorEditor()
{
}

void PMixGenericAudioProcessorEditor::paint (Graphics& g)
{
  //g.fillAll (Colours::white);
}

void PMixGenericAudioProcessorEditor::resized()
{
  panel.setBounds (getLocalBounds());
}

