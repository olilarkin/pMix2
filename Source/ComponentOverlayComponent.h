#ifndef COMPONENTOVERLAY_H_INCLUDED
#define COMPONENTOVERLAY_H_INCLUDED

#include "PMixDocument.h"
#include "ComponentLayout.h"

class ComponentOverlayComponent  : public Component,
                                   public ComponentListener,
                                   public ChangeListener,
                                   public ComponentBoundsConstrainer
{
public:
    ComponentOverlayComponent (Component* const targetComponent,
                               ComponentLayout& layout);

    ~ComponentOverlayComponent();

    virtual void showPopupMenu();

    void paint (Graphics& g);
    void resized();

    void mouseDown (const MouseEvent& e);
    void mouseDrag (const MouseEvent& e);
    void mouseUp (const MouseEvent& e);

    void componentMovedOrResized (Component& component, bool wasMoved, bool wasResized);

    void changeListenerCallback (ChangeBroadcaster*);

    void resizeStart();
    void resizeEnd();

    void updateBoundsToMatchTarget();

    void checkBounds (Rectangle<int>& bounds,
                      const Rectangle<int>& previousBounds,
                      const Rectangle<int>& limits,
                      bool isStretchingTop,
                      bool isStretchingLeft,
                      bool isStretchingBottom,
                      bool isStretchingRight);

    void applyBoundsToComponent (Component* component, const Rectangle<int>& bounds);

    Component::SafePointer<Component> target;
    const int borderThickness;

private:
    ScopedPointer<ResizableBorderComponent> border;

    ComponentLayout& layout;

    bool selected, dragging, mouseDownSelectStatus;
    double originalAspectRatio;
};

#endif   // COMPONENTOVERLAY_H_INCLUDED
