/*
 *  ParamView.h
 *  pMix
 *
 *  Created by Oliver Larkin on 25/12/2013.
 *  Copyright 2013 OliLarkin. All rights reserved.
 *
 */

class DemoSliderPropertyComponent : public SliderPropertyComponent
{
public:
  DemoSliderPropertyComponent (const String& propertyName)
  : SliderPropertyComponent (propertyName, 0.0, 100.0, 0.001)
  {
    setValue (Random::getSystemRandom().nextDouble() * 42.0);
  }
  
  void setValue (double newValue) override
  {
    slider.setValue (newValue);
  }
  
private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DemoSliderPropertyComponent)
};

static Array<PropertyComponent*> createSliders (int howMany)
{
  Array<PropertyComponent*> comps;
  
  for (int i = 0; i < howMany; ++i)
    comps.add (new DemoSliderPropertyComponent ("Slider " + String (i + 1)));
  
  return comps;
}

class ParamView : public Component
{
public:  
  ParamView()
  {
    setOpaque (true);
    addAndMakeVisible (&concertinaPanel);
    
    {
      PropertyPanel* panel = new PropertyPanel ("Plug-ins");
//      panel->addSection ("Section 1", createSliders (4), true);
//      panel->addSection ("Section 2", createSliders (3), true);
      addPanel (panel);
    }
  }
  
  void paint (Graphics& g) override
  {
    g.fillAll (Colour::greyLevel (0.8f));
  }
  
  void addPanel (PropertyPanel* panel)
  {
    concertinaPanel.addPanel (-1, panel, true);
    concertinaPanel.setMaximumPanelSize (panel, panel->getTotalContentHeight());
  }
  
  void resized() override
  {
    concertinaPanel.setBounds (getLocalBounds().reduced (4));
  }
  
private:
  ConcertinaPanel concertinaPanel;
};