/*
  ==============================================================================

    pMixGenericEditor.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixGenericEditor.h"


PMixParamSlider::PMixParamSlider (PMixAudioEngine &audioEngine, AudioProcessor& p, int paramIdx, uint32 nodeId)
: audioEngine(audioEngine)
, owner (p)
, index (paramIdx)
, nodeId(nodeId)
{
  const int steps = owner.getParameterNumSteps (index);
  
  if (steps > 1 && steps < 0x7fffffff)
    setRange (0.0, 1.0, 1.0 / (steps - 1.0));
  else
    setRange (0.0, 1.0);
  
  setSliderStyle (Slider::LinearHorizontal);
  setTextBoxStyle (Slider::NoTextBox, false, 0, 0);
  //setTextBoxIsEditable (true);
  //setPopupMenuEnabled(true);
  setScrollWheelEnabled (false);
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

Colour PMixParamSlider::getSliderColour()
{
  Colour sliderColour;
  if (audioEngine.getDoc().getParameterIsInterpolated(nodeId, index))
    sliderColour = audioEngine.getDoc().getFilterColour(nodeId);
  else
    sliderColour = findColour (Slider::thumbColourId);
  
  return sliderColour;
}

PMixProcessorParameterPropertyComp::PMixProcessorParameterPropertyComp (PMixAudioEngine &audioEngine, const String& name, AudioProcessor& p, int paramIdx, uint32 nodeId)
: PropertyComponent (name)
, audioEngine(audioEngine)
, owner (p)
, index (paramIdx)
, paramHasChanged (false)
, slider (audioEngine, p, paramIdx, nodeId)
, nodeId(nodeId)
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

void PMixProcessorParameterPropertyComp::audioProcessorParameterChanged (AudioProcessor* p, int parameterIndex, float newValue)
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

void PMixProcessorParameterPropertyComp::mouseDown (const MouseEvent& e)
{
  bool isInterpolated = audioEngine.getDoc().getParameterIsInterpolated(nodeId, index);
  
  if (e.mods.isPopupMenu())
  {
    PopupMenu m;

    if (isInterpolated)
      m.addItem (1, "Don't Interpolate Parameter");
    else
      m.addItem (1, "Interpolate Parameter");
    
    const int r = m.show();
    
    if (r == 1)
    {
      audioEngine.getDoc().setParameterToInterpolate(nodeId, index, !isInterpolated);
      repaint();
    }
  }
  else if(e.mods.isShiftDown())
  {
    audioEngine.getDoc().setParameterToInterpolate(nodeId, index, !isInterpolated);
    repaint();
  }
}

PMixGenericAudioProcessorEditor::PMixGenericAudioProcessorEditor (PMixAudioEngine &audioEngine, AudioProcessor* const p, uint32 nodeId)
: AudioProcessorEditor (p)
, audioEngine(audioEngine)
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
    
    PMixProcessorParameterPropertyComp* const pc = new PMixProcessorParameterPropertyComp (audioEngine, name, *p, i, nodeId);
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

