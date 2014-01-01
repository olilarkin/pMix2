
#ifndef _ISPACE_H_
#define _ISPACE_H_

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

class iSpaceComponent  : public Component
                       , public LassoSource<Component*>  
{
private:
  
  class iSpacePreset  : public Component
  {
  private:
    ComponentDragger myDragger;
    ComponentBoundsConstrainer boundsConstrainer;
    UndoManager& undoManager;
    Rectangle<int> startBounds;
    Rectangle<int> endBounds;
    
    class MovePresetAction  : public UndoableAction
    {
    public:
      MovePresetAction (Component* iSpace, const String& componentID, Rectangle<int> startBounds, Rectangle<int> endBounds) noexcept
      : iSpace(iSpace)
      , componentID(componentID)
      , startBounds(startBounds)
      , endBounds(endBounds)
      {
      }
      
      bool perform()
      {
        iSpace->findChildWithID(componentID)->setBounds(endBounds);
        return true;
      }
      
      bool undo()
      {      
        iSpace->findChildWithID(componentID)->setBounds(startBounds);
        return true;
      }
      
      int getSizeInUnits()
      {
        return (int) sizeof (*this); //xxx should be more accurate
      }
      
    private:
      Component* iSpace;
      String componentID;
      Rectangle<int> startBounds;
      Rectangle<int> endBounds;
      JUCE_DECLARE_NON_COPYABLE (MovePresetAction)
    };
    
  public:
    iSpacePreset(UndoManager& undoManager)
    : undoManager (undoManager)
    {
    }

    ~iSpacePreset ()
    {
    }

    void resized ()
    {
      int radius = getWidth()/2;
      boundsConstrainer.setMinimumOnscreenAmounts(radius,radius,radius,radius);
    }

    void mouseDown (const MouseEvent& e)
    {
      myDragger.startDraggingComponent (this, e);
      toFront (true);
      startBounds = getBounds();
    }

    void mouseDrag (const MouseEvent& e)
    {
      myDragger.dragComponent (this, e, &boundsConstrainer);
    }

    void mouseUp (const MouseEvent& e)
    {
      endBounds = getBounds();
      
      undoManager.beginNewTransaction();
      undoManager.perform(new MovePresetAction(getParentComponent(), getComponentID(), startBounds, endBounds), "change preset bounds");
    }

    void paint (Graphics& g)
    {
      g.setColour(Colours::red);
      g.fillEllipse (0, 0, getWidth(), getHeight());
    }
  };
  
  //TooltipWindow tooltipWindow;
  UndoManager& undoManager;
  Random mRand;
  SelectedItemSet<Component*> selectedItems;
  LassoComponent<Component*> lassoComp;
  
public:
  iSpaceComponent (UndoManager& undoManager)
  : undoManager (undoManager)
  , mRand(Time::currentTimeMillis())
  {
    for(int i = 0; i<10; i++)
    {
      iSpacePreset* const comp = new iSpacePreset(undoManager);
      comp->setComponentID(String("preset") + String(i));
      addAndMakeVisible (comp);
    }
  }

  ~iSpaceComponent ()
  {
    deleteAllChildren();
  }

  void resized ()
  {
    for (int i = 0; i < getNumChildComponents(); ++i)
    {
      iSpacePreset* const comp = dynamic_cast <iSpacePreset*> (getChildComponent(i));

      float r = 50. + (50. * mRand.nextFloat());
      float x = getWidth() * mRand.nextFloat();
      float y = getHeight() * mRand.nextFloat();
      comp->setBounds(x, y, r, r);
    }
  }

  void paint (Graphics& g)
  {
    g.fillAll (Colours::white);
  }
  
  void mouseDown (const MouseEvent& e)
  {
    addChildComponent (lassoComp);
    lassoComp.beginLasso (e, this);
  }
  
  void mouseDrag (const MouseEvent& e)
  {
    lassoComp.toFront (false);
    lassoComp.dragLasso (e);
  }
  
  void mouseUp (const MouseEvent& e)
  {
    lassoComp.endLasso();
    removeChildComponent (&lassoComp);
  }
  
  //LassoSource
  void findLassoItemsInArea (Array <Component*>& results, const Rectangle<int>& area)
  {
    //  const Rectangle<int> lasso (area - subCompHolder->getPosition());
    //  
    //  for (int i = 0; i < subCompHolder->getNumChildComponents(); ++i)
    //  {
    //    Component* c = subCompHolder->getChildComponent (i);
    //    
    //    if (c->getBounds().intersects (lasso))
    //      results.add (c);
    //  }
  }
  
  SelectedItemSet <Component*>& getLassoSelection()
  {
    return selectedItems;
  }
};

#endif//_ISPACE_H_ 