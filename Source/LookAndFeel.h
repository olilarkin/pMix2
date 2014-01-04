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