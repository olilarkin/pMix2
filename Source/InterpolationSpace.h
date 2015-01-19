
#ifndef _INTERPOLATIONSPACE_H_
#define _INTERPOLATIONSPACE_H_

#include "../JuceLibraryCode/JuceHeader.h"

//class iSpaceComponent  : public Component
//                       , private OpenGLRenderer
//{
//public:
//  iSpaceComponent()
//  {
//    openGLContext.setRenderer (this);
//    openGLContext.attachTo (*this);
//    openGLContext.setContinuousRepainting (true);
//  }
//  
//  ~iSpaceComponent()
//  {
//    openGLContext.detach();
//  }
//  
//  void newOpenGLContextCreated() override
//  {
//  }
//  
//  void openGLContextClosing() override
//  {
//  }
//  
//  void renderOpenGL() override
//  {
//    jassert (OpenGLHelpers::isContextActive());
//    const float desktopScale = (float) openGLContext.getRenderingScale();
//    
//    OpenGLHelpers::clear (Colours::lightblue);
//    
//    // Create an OpenGLGraphicsContext that will draw into this GL window..
//    ScopedPointer<LowLevelGraphicsContext> glRenderer (createOpenGLGraphicsContext (openGLContext,
//                                                                                    roundToInt (desktopScale * getWidth()),
//                                                                                    roundToInt (desktopScale * getHeight())));
//    
//    if (glRenderer != nullptr)
//    {
//      Graphics g (*glRenderer);
//      g.addTransform (AffineTransform::scale (desktopScale));
//      g.setColour(Colours::red);
//      g.fillEllipse (100., 100., 50., 50.);
//    }
//  }
//
//  void paint (Graphics& g)
//  {
//  }
//  
//private:
//  OpenGLContext openGLContext;
//};

class iSpaceLabel : public Label
{
public:
  iSpaceLabel(const String& labelText);
};

class MovePresetAction : public UndoableAction
{
public:
  MovePresetAction (Component* iSpace, const String& componentID, Rectangle<int> startBounds, Rectangle<int> endBounds) noexcept;
  bool perform();
  bool undo();
  int getSizeInUnits();
  
private:
  Component* iSpace;
  String componentID;
  Rectangle<int> startBounds;
  Rectangle<int> endBounds;
  JUCE_DECLARE_NON_COPYABLE (MovePresetAction)
};

class iSpacePreset : public Component
{
private:
  ComponentDragger myDragger;
  ComponentBoundsConstrainer boundsConstrainer;
  UndoManager& undoManager;
  Rectangle<int> startBounds;
  Rectangle<int> endBounds;
  iSpaceLabel* label;
    
public:
  iSpacePreset(UndoManager& undoManager, String& initalLabel);
  ~iSpacePreset ();
  void resized ();
  void mouseDown (const MouseEvent& e);
  void mouseDrag (const MouseEvent& e);
  void mouseUp (const MouseEvent& e);
  void paint (Graphics& g);
};

class iSpaceComponent  : public Component
                       , public LassoSource<Component*>  
{
private:
  //TooltipWindow tooltipWindow;
  UndoManager& undoManager;
  Random mRand;
  SelectedItemSet<Component*> selectedItems;
  LassoComponent<Component*> lassoComp;
  
public:
  iSpaceComponent (UndoManager& undoManager);
  ~iSpaceComponent ();
  void resized ();
  void paint (Graphics& g);
  void mouseDown (const MouseEvent& e);
  void mouseDrag (const MouseEvent& e);
  void mouseUp (const MouseEvent& e);
  void findLassoItemsInArea (Array <Component*>& results, const Rectangle<int>& area);
  SelectedItemSet <Component*>& getLassoSelection();
};

#endif //_INTERPOLATIONSPACE_H_ 