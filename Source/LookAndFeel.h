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
    
    g.drawFittedText (component.getName(),
                      3, r.getY(), r.getX() - 5, r.getHeight(),
                      Justification::centredLeft, 2);
  }
  
  Font getPopupMenuFont()
  {
    return Font (12.0f);
  }
  
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LookAndFeelCustom);
};

#endif   // __JUCE_LookAndFeelCustom_JUCEHEADER__