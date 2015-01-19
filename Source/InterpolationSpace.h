
#ifndef _INTERPOLATIONSPACE_H_
#define _INTERPOLATIONSPACE_H_

#include "../JuceLibraryCode/JuceHeader.h"

//class InterpolationSpaceComponent  : public Component
//                       , private OpenGLRenderer
//{
//public:
//  InterpolationSpaceComponent()
//  {
//    openGLContext.setRenderer (this);
//    openGLContext.attachTo (*this);
//    openGLContext.setContinuousRepainting (true);
//  }
//  
//  ~InterpolationSpaceComponent()
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

class InterpolationSpaceLabel : public Label
{
public:
  InterpolationSpaceLabel(const String& labelText);
};

class MovePresetAction : public UndoableAction
{
public:
  MovePresetAction (Component* interpolationSpace, const String& componentID, Rectangle<int> startBounds, Rectangle<int> endBounds) noexcept;
  bool perform();
  bool undo();
  int getSizeInUnits();
  
private:
  Component* interpolationSpace;
  String componentID;
  Rectangle<int> startBounds;
  Rectangle<int> endBounds;
  JUCE_DECLARE_NON_COPYABLE (MovePresetAction)
};

class InterpolationSpacePreset : public Component
{
private:
  ComponentDragger myDragger;
  ComponentBoundsConstrainer boundsConstrainer;
  UndoManager& undoManager;
  Rectangle<int> startBounds;
  Rectangle<int> endBounds;
  InterpolationSpaceLabel* label;
    
public:
  InterpolationSpacePreset(UndoManager& undoManager, String& initalLabel);
  ~InterpolationSpacePreset ();
  void resized ();
  void mouseDown (const MouseEvent& e);
  void mouseDrag (const MouseEvent& e);
  void mouseUp (const MouseEvent& e);
  void paint (Graphics& g);
};

class InterpolationSpaceComponent  : public Component
                       , public LassoSource<Component*>  
{
private:
  //TooltipWindow tooltipWindow;
  UndoManager& undoManager;
  Random mRand;
  SelectedItemSet<Component*> selectedItems;
  LassoComponent<Component*> lassoComp;
  
public:
  InterpolationSpaceComponent (UndoManager& undoManager);
  ~InterpolationSpaceComponent ();
  void resized ();
  void paint (Graphics& g);
  void mouseDown (const MouseEvent& e);
  void mouseDrag (const MouseEvent& e);
  void mouseUp (const MouseEvent& e);
  void findLassoItemsInArea (Array <Component*>& results, const Rectangle<int>& area);
  SelectedItemSet <Component*>& getLassoSelection();
};

#endif //_INTERPOLATIONSPACE_H_ 