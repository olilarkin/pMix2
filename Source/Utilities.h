/*
  ==============================================================================

    Utilities.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#ifndef UTILITIES_H_INCLUDED
#define UTILITIES_H_INCLUDED


inline void drawResizableBorder (Graphics& g, int w, int h,
                                 const BorderSize<int> borderSize,
                                 const bool isMouseOver)
{
    g.setColour (Colours::orange.withAlpha (isMouseOver ? 0.4f : 0.3f));

    g.fillRect (0, 0, w, borderSize.getTop());
    g.fillRect (0, 0, borderSize.getLeft(), h);
    g.fillRect (0, h - borderSize.getBottom(), w, borderSize.getBottom());
    g.fillRect (w - borderSize.getRight(), 0, borderSize.getRight(), h);

    g.drawRect (borderSize.getLeft() - 1, borderSize.getTop() - 1,
                w - borderSize.getRight() - borderSize.getLeft() + 2,
                h - borderSize.getTop() - borderSize.getBottom() + 2);
}

inline void drawMouseOverCorners (Graphics& g, int w, int h)
{
    RectangleList<int> r (Rectangle<int> (0, 0, w, h));
    r.subtract (Rectangle<int> (1, 1, w - 2, h - 2));

    const int size = jmin (w / 3, h / 3, 12);
    r.subtract (Rectangle<int> (size, 0, w - size - size, h));
    r.subtract (Rectangle<int> (0, size, w, h - size - size));

    g.setColour (Colours::darkgrey);

    for (int i = r.getNumRectangles(); --i >= 0;)
        g.fillRect (r.getRectangle (i));
}


#endif  // UTILITIES_H_INCLUDED
