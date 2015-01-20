#ifndef EDITINGPANELBASE_H_INCLUDED
#define EDITINGPANELBASE_H_INCLUDED

#include "PMixDocument.h"
#include "ComponentLayoutEditor.h"
class LayoutPropsPanel;

class EditingPanelBase  : public Component
{
public:
    EditingPanelBase (PMixDocument& document,
                      Component* propsPanel,
                      Component* editorComp);

    ~EditingPanelBase();

    void resized();
    void visibilityChanged();

    virtual void updatePropertiesList() = 0;

    virtual Rectangle<int> getComponentArea() const = 0;

    double getZoom() const;
    void setZoom (double newScale);
    void setZoom (double newScale, int anchorX, int anchorY);

    // convert a pos relative to this component into a pos on the editor
    void xyToTargetXY (int& x, int& y) const;

    void dragKeyHeldDown (bool isKeyDown);

    class MagnifierComponent;

protected:
    PMixDocument& document;
    LookAndFeel_V2 lookAndFeel;

    Viewport* viewport;
    MagnifierComponent* magnifier;
    Component* editor;
    Component* propsPanel;
};

#endif   // EDITINGPANELBASE_H_INCLUDED
