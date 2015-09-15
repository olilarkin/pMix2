#include "pMixLookAndFeel.h"
#include "pMixGenericEditor.h"

namespace LookAndFeelHelpers
{
  static Colour createBaseColour (Colour buttonColour,
                                  bool hasKeyboardFocus,
                                  bool isMouseOverButton,
                                  bool isButtonDown) noexcept
  {
    const float sat = hasKeyboardFocus ? 1.3f : 0.9f;
    const Colour baseColour (buttonColour.withMultipliedSaturation (sat));
    
    if (isButtonDown)      return baseColour.contrasting (0.2f);
      if (isMouseOverButton) return baseColour.contrasting (0.1f);
        
        return baseColour;
  }
  
  static TextLayout layoutTooltipText (const String& text, Colour colour) noexcept
  {
    const float tooltipFontSize = 13.0f;
    const int maxToolTipWidth = 400;
    
    AttributedString s;
    s.setJustification (Justification::centred);
    s.append (text, Font (tooltipFontSize, Font::bold), colour);
    
    TextLayout tl;
    tl.createLayoutWithBalancedLineLengths (s, (float) maxToolTipWidth);
    return tl;
  }
}

pMixLookAndFeel::pMixLookAndFeel() {};

void pMixLookAndFeel::drawPropertyComponentLabel (Graphics& g, int width, int height, PropertyComponent& component)
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

void pMixLookAndFeel::drawLinearSliderBackground (Graphics& g, int x, int y, int width, int height,
                                                 float sliderPos, float minSliderPos, float maxSliderPos,
                                                 const Slider::SliderStyle, Slider& slider)
{
  const float sliderRadius = (float) (getSliderThumbRadius (slider) - 3);
  
  const Colour trackColour (slider.findColour (Slider::trackColourId));
  //const Colour trackColour = Colours::darkgrey;

  const Colour gradCol1 (trackColour.overlaidWith (Colour (slider.isEnabled() ? 0x13000000 : 0x09000000)));
  const Colour gradCol2 (trackColour.overlaidWith (Colour (0x06000000)));
  //Path indent;
  
  if (slider.isHorizontal())
  {
    g.setColour (trackColour);
    const float iy = y + height * 0.5f - sliderRadius * 0.5f;
    g.fillRect((float)x, iy, (float)width, (float)4);
//
//
//    //g.setGradientFill (ColourGradient (gradCol1, 0.0f, iy,
//    //                                   gradCol2, 0.0f, iy + sliderRadius, false));
//    
//    indent.addRoundedRectangle (x - sliderRadius * 0.5f, iy, width + sliderRadius, sliderRadius, 5.0f);
  }
//  else
//  {
//    const float ix = x + width * 0.5f - sliderRadius * 0.5f;
//    
//    
//    //g.setGradientFill (ColourGradient (gradCol1, ix, 0.0f,
//    //                                   gradCol2, ix + sliderRadius, 0.0f, false));
//    
//    indent.addRoundedRectangle (ix, y - sliderRadius * 0.5f, sliderRadius, height + sliderRadius, 5.0f);
//  }
  
  //g.fillPath (indent);
  
  //g.setColour (trackColour.contrasting (0.5f));
  //g.strokePath (indent, PathStrokeType (0.5f));
}

void pMixLookAndFeel::drawLinearSliderThumb (Graphics& g, int x, int y, int width, int height,
                                            float sliderPos, float minSliderPos, float maxSliderPos,
                                            const Slider::SliderStyle style, Slider& slider)
{
  const float sliderRadius = (float) (getSliderThumbRadius (slider) - 2);
  
  PMixParamSlider * pmixslider = dynamic_cast<PMixParamSlider*>(&slider);
  Colour knobColour;
  
  if (pmixslider)
  {
    knobColour = (LookAndFeelHelpers::createBaseColour (pmixslider->getSliderColour(),
                                                        slider.hasKeyboardFocus (false) && slider.isEnabled(),
                                                        slider.isMouseOverOrDragging() && slider.isEnabled(),
                                                        slider.isMouseButtonDown() && slider.isEnabled()));  }
  else
  {
    knobColour = (LookAndFeelHelpers::createBaseColour (slider.findColour (Slider::thumbColourId),
                                                             slider.hasKeyboardFocus (false) && slider.isEnabled(),
                                                             slider.isMouseOverOrDragging() && slider.isEnabled(),
                                                             slider.isMouseButtonDown() && slider.isEnabled()));
    
  }
  
  //const float outlineThickness = slider.isEnabled() ? 0.8f : 0.3f;
  
  if (style == Slider::LinearHorizontal || style == Slider::LinearVertical)
  {
    float kx, ky;
    
    if (style == Slider::LinearVertical)
    {
      kx = x + width * 0.5f;
      ky = sliderPos;
    }
    else
    {
      kx = sliderPos;
      ky = y + height * 0.5f;
    }
    
    Path p;
    p.addEllipse (kx - sliderRadius, ky - sliderRadius, sliderRadius * 2.0f, sliderRadius * 2.0f);
    g.setColour(knobColour);
    g.fillPath (p);
    
    g.setColour(Colours::darkgrey);
    g.drawEllipse(kx - sliderRadius, ky - sliderRadius, sliderRadius * 2.0f, sliderRadius * 2.0f, 1.f);
//    drawGlassSphere (g,
//                     kx - sliderRadius,
//                     ky - sliderRadius,
//                     sliderRadius * 2.0f,
//                     knobColour, outlineThickness);
  }
//  else
//  {
//    if (style == Slider::ThreeValueVertical)
//    {
//      drawGlassSphere (g, x + width * 0.5f - sliderRadius,
//                       sliderPos - sliderRadius,
//                       sliderRadius * 2.0f,
//                       knobColour, outlineThickness);
//    }
//    else if (style == Slider::ThreeValueHorizontal)
//    {
//      drawGlassSphere (g,sliderPos - sliderRadius,
//                       y + height * 0.5f - sliderRadius,
//                       sliderRadius * 2.0f,
//                       knobColour, outlineThickness);
//    }
//    
//    if (style == Slider::TwoValueVertical || style == Slider::ThreeValueVertical)
//    {
//      const float sr = jmin (sliderRadius, width * 0.4f);
//      
//      drawGlassPointer (g, jmax (0.0f, x + width * 0.5f - sliderRadius * 2.0f),
//                        minSliderPos - sliderRadius,
//                        sliderRadius * 2.0f, knobColour, outlineThickness, 1);
//      
//      drawGlassPointer (g, jmin (x + width - sliderRadius * 2.0f, x + width * 0.5f), maxSliderPos - sr,
//                        sliderRadius * 2.0f, knobColour, outlineThickness, 3);
//    }
//    else if (style == Slider::TwoValueHorizontal || style == Slider::ThreeValueHorizontal)
//    {
//      const float sr = jmin (sliderRadius, height * 0.4f);
//      
//      drawGlassPointer (g, minSliderPos - sr,
//                        jmax (0.0f, y + height * 0.5f - sliderRadius * 2.0f),
//                        sliderRadius * 2.0f, knobColour, outlineThickness, 2);
//      
//      drawGlassPointer (g, maxSliderPos - sliderRadius,
//                        jmin (y + height - sliderRadius * 2.0f, y + height * 0.5f),
//                        sliderRadius * 2.0f, knobColour, outlineThickness, 4);
//    }
//  }
}

void pMixLookAndFeel::drawLasso (Graphics& g, Component& lassoComp)
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

Font pMixLookAndFeel::getPopupMenuFont()
{
  return Font (12.0f);
}

void pMixLookAndFeel::drawStretchableLayoutResizerBar (Graphics& g, int /*w*/, int /*h*/, bool /*isVerticalBar*/,
                                                      bool isMouseOver, bool isMouseDragging)
{
  if (isMouseOver || isMouseDragging)
    g.fillAll (Colours::yellow.withAlpha (0.4f));
  else
    g.fillAll (Colours::lightgrey);
}

void pMixLookAndFeel::drawLabel (Graphics& g, Label& label)
{
  g.fillAll (label.findColour (Label::backgroundColourId));
  
  if (! label.isBeingEdited())
  {
    const float alpha = label.isEnabled() ? 1.0f : 0.5f;
    const Font font (getLabelFont (label));
    
    g.setColour (label.findColour (Label::textColourId).withMultipliedAlpha (alpha));
    g.setFont (font);
    
    Rectangle<int> textArea (label.getBorderSize().subtractedFrom (label.getLocalBounds()));
    
    g.drawFittedText (label.getText(), textArea, label.getJustificationType(),
                      jmax (1, (int) (textArea.getHeight() / font.getHeight())),
                      /*label.getMinimumHorizontalScale()*/ 1.f);
    
    g.setColour (label.findColour (Label::outlineColourId).withMultipliedAlpha (alpha));
  }
  else if (label.isEnabled())
  {
    g.setColour (label.findColour (Label::outlineColourId));
  }
  
  g.drawRect (label.getLocalBounds());
}

