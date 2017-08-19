
#pragma once

#include "JuceHeader.h"

class pMixLookAndFeel : public LookAndFeel_V3
{
public:
  pMixLookAndFeel();
  void drawPropertyComponentLabel (Graphics& g, int width, int height, PropertyComponent& component);
  void drawLinearSliderBackground (Graphics& g, int x, int y, int width, int height,
                                   float sliderPos, float minSliderPos, float maxSliderPos,
                                   const Slider::SliderStyle, Slider& slider);
  void drawLinearSliderThumb (Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider& slider);
  void drawLasso (Graphics& g, Component& lassoComp);
  Font getPopupMenuFont();
  void drawStretchableLayoutResizerBar (Graphics& g, int /*w*/, int /*h*/, bool /*isVerticalBar*/,
                                        bool isMouseOver, bool isMouseDragging);
  
  void drawLabel(Graphics&, Label&);
  void drawCallOutBoxBackground (CallOutBox &, Graphics &, const Path &, Image &cachedImage);
private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pMixLookAndFeel);
};

