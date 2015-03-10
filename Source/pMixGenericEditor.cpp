/*
  ==============================================================================

    pMixGenericEditor.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixGenericEditor.h"

class PMixProcessorParameterPropertyComp   : public PropertyComponent,
private AudioProcessorListener,
private Timer
{
public:
  PMixProcessorParameterPropertyComp (const String& name, AudioProcessor& p, int paramIndex)
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
  
  ~PMixProcessorParameterPropertyComp()
  {
    owner.removeListener (this);
  }
  
  void refresh() override
  {
    paramHasChanged = false;
    
    if (slider.getThumbBeingDragged() < 0)
      slider.setValue (owner.getParameter (index), dontSendNotification);
    
    slider.updateText();
  }
  
  void audioProcessorChanged (AudioProcessor*) override  {}
  
  void audioProcessorParameterChanged (AudioProcessor*, int parameterIndex, float) override
  {
    if (parameterIndex == index)
      paramHasChanged = true;
  }
  
  void timerCallback() override
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
  
private:
  class PMixParamSlider  : public Slider
  {
  public:
    PMixParamSlider (AudioProcessor& p, int paramIndex)  : owner (p), index (paramIndex)
    {
      const int steps = owner.getParameterNumSteps (index);
      
      if (steps > 1 && steps < 0x7fffffff)
        setRange (0.0, 1.0, 1.0 / (steps - 1.0));
      else
        setRange (0.0, 1.0);
      
      setSliderStyle (Slider::LinearHorizontal);
      setTextBoxStyle (Slider::NoTextBox, false, 0, 0);
      //setTextBoxIsEditable (true);
      setScrollWheelEnabled (true);
    }
    
    void valueChanged() override
    {
      const float newVal = (float) getValue();
      
      if (owner.getParameter (index) != newVal)
      {
        owner.setParameterNotifyingHost (index, newVal);
        updateText();
      }
    }
    
    String getTextFromValue (double value) override
    {
      return owner.getParameterText (index) + " " + owner.getParameterLabel (index).trimEnd();
    }
    
  private:
    AudioProcessor& owner;
    const int index;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PMixParamSlider)
  };
  
  AudioProcessor& owner;
  const int index;
  bool volatile paramHasChanged;
  PMixParamSlider slider;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PMixProcessorParameterPropertyComp)
};

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

