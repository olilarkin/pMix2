
#ifndef _ISPACE_H_
#define _ISPACE_H_

class iSpaceComponent;

class iSpacePreset  : public Component
{
private:
  Point<int> originalPos;

  iSpaceComponent* getiSpace() const throw()
  {
    return findParentComponentOfClass<iSpaceComponent>();
  }


public:
  iSpacePreset ()
  {
  }

  ~iSpacePreset ()
  {
  }

  void resized ()
  {
  }

  void mouseDown (const MouseEvent& e)
  {
    originalPos = localPointToGlobal (Point<int>());

    toFront (true);
  }

  void mouseDrag (const MouseEvent& e)
  {
    if (! e.mods.isPopupMenu())
    {
      Point<int> pos (originalPos + Point<int> (e.getDistanceFromDragStartX(), e.getDistanceFromDragStartY()));

      if (getParentComponent() != 0)
        pos = getParentComponent()->globalPositionToRelative (pos);

      setBounds(pos.getX(), pos.getY(), getWidth(), getHeight());
    }
  }

  void mouseUp (const MouseEvent& e)
  {
  }

  void paint (Graphics& g)
  {
    g.setColour(Colours::red);

    g.fillEllipse (0, 0, getWidth(), getHeight());
  }

  //bool hitTest(int x, int y)
  //{
  //
  //}

};

class iSpaceComponent  : public Component
{
private:
  //TooltipWindow tooltipWindow;
  Random* mRand;
public:
  iSpaceComponent ()
  {
    mRand = new Random (Time::currentTimeMillis());

    for(int i = 0; i<10; i++)
    {
      iSpacePreset* const comp = new iSpacePreset();
      addAndMakeVisible (comp);
      // comp->update();
    }
  }

  ~iSpaceComponent ()
  {
    delete mRand;
    deleteAllChildren();
  }

  void resized ()
  {
    for (int i = 0; i < getNumChildComponents(); ++i)
    {
      iSpacePreset* const comp = dynamic_cast <iSpacePreset*> (getChildComponent(i));

      float r = 50 * mRand->nextFloat();
      float x = getWidth() * mRand->nextFloat();
      float y = getHeight() * mRand->nextFloat();
      comp->setBounds(x, y, r, r);
    }
  }

  void paint (Graphics& g)
  {
    g.fillAll (Colours::grey);
    /*
    g.setColour(Colours::red);

    for(int i = 0; i<100; i++)
    {
      float r = 50 * mRand->nextFloat();
      g.fillEllipse (getWidth() * mRand->nextFloat(), getHeight() * mRand->nextFloat(), r, r);
    }
    */
  }


};

#endif//_ISPACE_H_ 