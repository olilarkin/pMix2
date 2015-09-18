/*
  ==============================================================================

    pMixInterpolationSpaceCrosshairs.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixInterpolationSpaceCrosshairs.h"

InterpolationSpaceIPos::InterpolationSpaceIPos(PMixAudioEngine& audioEngine, PMixInterpolationSpaceLayout& layout, const uint32 nodeId, Colour colour)
: audioEngine(audioEngine)
, layout(layout)
, colour(colour)
, nodeId(nodeId)
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
  setMouseCursor (MouseCursor::NoCursor);
  myDragger.startDraggingComponent (this, e);
  toFront (true);
}

void InterpolationSpaceIPos::mouseDrag (const MouseEvent& e)
{
  Point<double> normalizedPos;
  Rectangle<int> bounds = getBounds();
  normalizedPos.x = bounds.getCentreX()  / (double) getParentWidth();
  normalizedPos.y = bounds.getCentreY() / (double) getParentHeight();
  audioEngine.getDoc().setFilterIPos(nodeId, normalizedPos.x, normalizedPos.y);
  
  myDragger.dragComponent (this, e, &boundsConstrainer);
  
  layout.repaintPresetsForFilter(nodeId);
}

void InterpolationSpaceIPos::mouseUp (const MouseEvent& e)
{
  setMouseCursor (MouseCursor::NormalCursor);
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

void InterpolationSpaceIPos::update()
{
  const AudioProcessorGraph::Node::Ptr f (audioEngine.getDoc().getNodeForId (nodeId));
  
  if (f == nullptr)
  {
    delete this;
    return;
  }
  
  colour = audioEngine.getDoc().getFilterColour(nodeId);
  repaint();
}

pMixInterpolationSpaceCrossHairs::pMixInterpolationSpaceCrossHairs(PMixAudioEngine& audioEngine, PMixInterpolationSpaceLayout& layout)
: audioEngine(audioEngine)
, layout(layout)
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
  for (int i = getNumChildComponents(); --i >= 0;)
  {
    if (InterpolationSpaceIPos* const ic = dynamic_cast <InterpolationSpaceIPos*> (getChildComponent (i)))
    {
      const AudioProcessorGraph::Node::Ptr f (audioEngine.getDoc().getNodeForId(ic->nodeId));

      var iposx = f->properties["iposx"];
      var iposy = f->properties["iposy"];
      
      float x = getWidth() * (float) iposx;
      float y = getHeight() * (float) iposy;
      ic->setCentrePosition(x, y);
      
      ic->update();
    }
  }

  for (int i = audioEngine.getDoc().getNumFilters(); --i >= 0;)
  {
    const AudioProcessorGraph::Node::Ptr f (audioEngine.getDoc().getNode (i));
    
    if (getComponentForFilter (f->nodeId) == nullptr)
    {
      if (f->properties.getVarPointer("presets") != nullptr)
      {
        var iposx = f->properties["iposx"];
        var iposy = f->properties["iposy"];
        Array<var>* presets = f->properties.getVarPointer("presets")->getArray();

        if (presets->size() >= 2)
        {
          InterpolationSpaceIPos* comp = new InterpolationSpaceIPos(audioEngine, layout, f->nodeId, audioEngine.getDoc().getFilterColour(f->nodeId));
          float r = 25.f;
          float x = getWidth() * (float) iposx;
          float y = getHeight() * (float) iposy;
          comp->setSize(r*2.f, r*2.f);
          comp->setCentrePosition(x, y);
          comp->update();
          addAndMakeVisible(comp);
        }
      }
    }
  }
}

InterpolationSpaceIPos* pMixInterpolationSpaceCrossHairs::getComponentForFilter (const uint32 nodeId) const
{
  for (int i = getNumChildComponents(); --i >= 0;)
  {
    if (InterpolationSpaceIPos* const ic = dynamic_cast <InterpolationSpaceIPos*> (getChildComponent (i)))
      if (ic->nodeId == nodeId)
        return ic;
  }
  
  return nullptr;
}