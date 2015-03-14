/*
  ==============================================================================

    pMixInterpolationSpaceCrosshairs.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixInterpolationSpaceCrosshairs.h"

InterpolationSpaceIPos::InterpolationSpaceIPos(PMixAudioEngine& audioEngine, const uint32 filterID, Colour colour)
: audioEngine(audioEngine)
, filterID(filterID)
, colour(colour)
{
}

InterpolationSpaceIPos::~InterpolationSpaceIPos ()
{
}

void InterpolationSpaceIPos::resized ()
{
  int radius = getWidth()/2;
  boundsConstrainer.setMinimumOnscreenAmounts(radius, radius, radius, radius);
  //  label->centreWithSize(getWidth()-5, 20);
}

void InterpolationSpaceIPos::mouseDown (const MouseEvent& e)
{
  myDragger.startDraggingComponent (this, e);
  toFront (true);
}

void InterpolationSpaceIPos::mouseDrag (const MouseEvent& e)
{
  Point<double> normalizedPos;
  normalizedPos.x = getX() / (double) getParentWidth();
  normalizedPos.y = getY() / (double) getParentHeight();
  audioEngine.getDoc().setFilterIPos(filterID, normalizedPos.x, normalizedPos.y);
  
  myDragger.dragComponent (this, e, &boundsConstrainer);
  getParentComponent()->repaint();
}

void InterpolationSpaceIPos::mouseUp (const MouseEvent& e)
{
}

void InterpolationSpaceIPos::paint (Graphics& g)
{
  g.setColour(colour);

  PathStrokeType stroke (2.5f);

  Path linePath;
  
  float mw = getWidth()/2.f;
  float mh = getHeight()/2.f;

  linePath.startNewSubPath(0, mh);
  linePath.lineTo(getWidth(), mh);
  linePath.startNewSubPath(mw, 0);
  linePath.lineTo(mw, getHeight());
  g.strokePath(linePath, stroke);
  
  g.fillEllipse ((getWidth()/2.f) - 5.f, (getHeight()/2.f)  - 5.f, 10.f, 10.f);
  g.setColour(Colours::black);
  g.drawEllipse((getWidth()/2.f) - 5.f, (getHeight()/2.f)  - 5.f, 10.f, 10.f, 1.f);
}

pMixInterpolationSpaceCrossHairs::pMixInterpolationSpaceCrossHairs(PMixAudioEngine& audioEngine)
: audioEngine(audioEngine)
{
  setInterceptsMouseClicks(false, true);
  audioEngine.getDoc().addChangeListener (this);
}

pMixInterpolationSpaceCrossHairs::~pMixInterpolationSpaceCrossHairs()
{
  audioEngine.getDoc().removeChangeListener(this);
  deleteAllChildren();
}

void pMixInterpolationSpaceCrossHairs::changeListenerCallback (ChangeBroadcaster* source)
{
  updateComponents();
  repaint();
}

void pMixInterpolationSpaceCrossHairs::paint (Graphics& g)
{
//  g.setColour(Colours::black);
//  
//  PathStrokeType stroke (2.5f);
//  
//  for (int f=0; f<getNumChildComponents(); f++)
//  {
//    Array<Point<float>>& positions = presetsLocations.getReference(f);
//    Path linePath;
//
//    float mw = getChildComponent(f)->getX() + getChildComponent(f)->getWidth()/2.f;
//    float mh = getChildComponent(f)->getY() + getChildComponent(f)->getHeight()/2.f;
//    
//    for (int preset = 0; preset < positions.size(); preset++)
//    {
//      linePath.startNewSubPath(mw, mh);
//      linePath.lineTo(positions[preset]);
//    }
//
//    g.strokePath(linePath, stroke);
//  }
}

void pMixInterpolationSpaceCrossHairs::resized()
{
}

void pMixInterpolationSpaceCrossHairs::updateComponents()
{
  deleteAllChildren();
//  presetsLocations.clear();
  
  for (int i = audioEngine.getDoc().getNumFilters(); --i >= 0;)
  {
    const AudioProcessorGraph::Node::Ptr f (audioEngine.getDoc().getNode (i));
    
    if (f->properties.getVarPointer("presets") != nullptr)
    {
      var iposx = f->properties["iposx"];
      var iposy = f->properties["iposy"];
      Array<var>* presets = f->properties.getVarPointer("presets")->getArray();

//      Array<Point<float>> positions;
      
      if (presets->size() >= 2)
      {
        InterpolationSpaceIPos* comp = new InterpolationSpaceIPos(audioEngine, f->nodeId, audioEngine.getDoc().getFilterColour(f->nodeId));
        float r = 50.f;
        float x = getWidth() * (float) iposx;
        float y = getHeight() * (float) iposy;
        comp->setBounds(x, y, r, r);
        
        
//        for (int p=0; p<presets->size(); p++)
//        {
//          DynamicObject* obj = presets->getReference(p).getDynamicObject();
//
//          positions.add(Point<float>((float)obj->getProperty("x")*getWidth(), (float)obj->getProperty("y")*getHeight()));
//        }
//        
        addAndMakeVisible(comp);
      }
//
//      presetsLocations.add(positions);
    }
  }
}