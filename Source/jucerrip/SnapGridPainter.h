#ifndef SNAPGRIDPAINTER_H_INCLUDED
#define SNAPGRIDPAINTER_H_INCLUDED

#include "PMixDocument.h"
//#include "../jucer_PaintRoutine.h"

class SnapGridPainter
{
public:
    SnapGridPainter()
        : snapGridSize (-1), snapShown (false)
    {
    }

    bool updateFromDesign (PMixDocument& design)
    {
        if (snapGridSize != design.getSnappingGridSize()
             || snapShown != (design.isSnapShown() && design.isSnapActive (false)))
        {
            snapGridSize = design.getSnappingGridSize();
            snapShown = design.isSnapShown() && design.isSnapActive (false);
            return true;
        }

        return false;
    }

    void draw (Graphics& g/*, PaintRoutine* backgroundGraphics*/)
    {
        if (snapShown && snapGridSize > 2)
        {
            Colour col (Colours::black);

//            if (backgroundGraphics != nullptr)
//                col = backgroundGraphics->getBackgroundColour().contrasting();

            const Rectangle<int> clip (g.getClipBounds());

            RectangleList<float> gridLines;

            for (int x = clip.getX() - (clip.getX() % snapGridSize); x < clip.getRight(); x += snapGridSize)
                gridLines.addWithoutMerging (Rectangle<float> ((float) x, 0.0f, 1.0f, (float) clip.getBottom()));

            for (int y = clip.getY() - (clip.getY() % snapGridSize); y < clip.getBottom(); y += snapGridSize)
                gridLines.addWithoutMerging (Rectangle<float> (0.0f, (float) y, (float) clip.getRight(), 1.0f));

            g.setColour (col.withAlpha (0.1f));
            g.fillRectList (gridLines);
        }
    }

private:
    int snapGridSize;
    bool snapShown;
};


#endif   // SNAPGRIDPAINTER_H_INCLUDED
