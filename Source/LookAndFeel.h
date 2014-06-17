#ifndef __JUCE_LookAndFeelCustom_JUCEHEADER__
#define __JUCE_LookAndFeelCustom_JUCEHEADER__

class LookAndFeelCustom : public LookAndFeel_V3
{
public:

  LookAndFeelCustom() {};
  ~LookAndFeelCustom() {};

  void drawPropertyComponentLabel (Graphics& g, int, int height, PropertyComponent& component)
  {
    g.setColour (Colours::black);
    
    if (! component.isEnabled())
      g.setOpacity (0.6f);
    
    g.setFont (jmin (height, 24) * 0.65f);
    
    const Rectangle<int> r (getPropertyComponentContentPosition (component));
    
    g.drawText (component.getName(),
                      3, r.getY(), r.getX() - 5, r.getHeight(),
                      Justification::centredLeft, 2);
  }
  
  void drawLinearSliderBackground (Graphics& g, int x, int y, int width, int height,
                              float /*sliderPos*/,
                              float /*minSliderPos*/,
                              float /*maxSliderPos*/,
                              const Slider::SliderStyle /*style*/, Slider& slider)
  {
    const float sliderRadius = (float) (getSliderThumbRadius (slider) - 2);
    
    const Colour trackColour (slider.findColour (Slider::trackColourId));
    const Colour gradCol1 (trackColour.overlaidWith (Colour (slider.isEnabled() ? 0x13000000 : 0x09000000)));
    const Colour gradCol2 (trackColour.overlaidWith (Colour (0x06000000)));
    Path indent;
    
    if (slider.isHorizontal())
    {
      const float iy = y + height * 0.5f - sliderRadius * 0.5f;
      
      g.setColour (trackColour);
      //g.setGradientFill (ColourGradient (gradCol1, 0.0f, iy,
      //                                   gradCol2, 0.0f, iy + sliderRadius, false));
      
      indent.addRoundedRectangle (x - sliderRadius * 0.5f, iy, width + sliderRadius, sliderRadius, 5.0f);
    }
    else
    {
      const float ix = x + width * 0.5f - sliderRadius * 0.5f;
      
      
      //g.setGradientFill (ColourGradient (gradCol1, ix, 0.0f,
      //                                   gradCol2, ix + sliderRadius, 0.0f, false));
      
      indent.addRoundedRectangle (ix, y - sliderRadius * 0.5f, sliderRadius, height + sliderRadius, 5.0f);
    }
    
    g.fillPath (indent);
    
    //g.setColour (trackColour.contrasting (0.5f));
    //g.strokePath (indent, PathStrokeType (0.5f));
  }
  
  void drawLasso (Graphics& g, Component& lassoComp)
  {    
    g.fillAll (lassoComp.findColour (0x1000440 /*lassoFillColourId*/));
    g.setColour (lassoComp.findColour (0x1000441 /*lassoOutlineColourId*/));
    
    Rectangle<int> bounds = lassoComp.getLocalBounds();
    Path linePath;
    linePath.addRectangle(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
    
    PathStrokeType stroke (2.5f);
    float dashes[2] = { 4, 4 };
    stroke.createDashedStroke(linePath, linePath, dashes, 2);
    
    g.fillPath (linePath);
  }
  
  Font getPopupMenuFont()
  {
    return Font (12.0f);
  }
  
private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LookAndFeelCustom);
};

#endif   // __JUCE_LookAndFeelCustom_JUCEHEADER__