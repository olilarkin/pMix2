
#ifndef __pMixLookAndFeel_JUCEHEADER__
#define __pMixLookAndFeel_JUCEHEADER__

#include "JuceHeader.h"

class pMixLookAndFeel : public LookAndFeel_V3
{
public:
  pMixLookAndFeel();
  void drawPropertyComponentLabel (Graphics& g, int width, int height, PropertyComponent& component);
  void drawLinearSliderBackground (Graphics& g, int x, int y, int width, int height,
                                   float sliderPos, float minSliderPos, float maxSliderPos,
                                   const Slider::SliderStyle, Slider& slider);
  //void drawLinearSliderThumb (Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider& slider);
  void drawLasso (Graphics& g, Component& lassoComp);
  Font getPopupMenuFont();
  
private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pMixLookAndFeel);
};

#endif   // __pMixLookAndFeel_JUCEHEADER__