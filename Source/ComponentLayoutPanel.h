#ifndef COMPONENTLAYOUTPANEL_H_INCLUDED
#define COMPONENTLAYOUTPANEL_H_INCLUDED

#include "ComponentLayoutEditor.h"
#include "EditingPanelBase.h"

class ComponentLayoutPanel  : public EditingPanelBase
{
public:
        ComponentLayoutPanel (PMixDocument& doc, ComponentLayout& l)
        : EditingPanelBase (doc,
                            new LayoutPropsPanel (doc, l),
                            new ComponentLayoutEditor (doc, l)),
          layout (l)
    {
    }

    ~ComponentLayoutPanel()
    {
        deleteAllChildren();
    }

    void updatePropertiesList()
    {
        ((LayoutPropsPanel*) propsPanel)->updateList();
    }

    Rectangle<int> getComponentArea() const
    {
        return ((ComponentLayoutEditor*) editor)->getComponentArea();
    }

    Image createComponentSnapshot() const
    {
        return ((ComponentLayoutEditor*) editor)->createComponentLayerSnapshot();
    }

    ComponentLayout& layout;

private:
    class LayoutPropsPanel  : public Component,
                              public ChangeListener
    {
    public:
        LayoutPropsPanel (PMixDocument& doc, ComponentLayout& l)
            : document (doc), layout (l)
        {
            layout.getSelectedSet().addChangeListener (this);
            addAndMakeVisible (propsPanel);
        }

        ~LayoutPropsPanel()
        {
            layout.getSelectedSet().removeChangeListener (this);
            clear();
        }

        void resized()
        {
            propsPanel.setBounds (4, 4, getWidth() - 8, getHeight() - 8);
        }

        void changeListenerCallback (ChangeBroadcaster*)
        {
            updateList();
        }

        void clear()
        {
            propsPanel.clear();
        }

        void updateList()
        {
            clear();

            if (layout.getSelectedSet().getNumSelected() == 1) // xxx need to cope with multiple
            {
                if (Component* comp = layout.getSelectedSet().getSelectedItem (0))
                    if (ComponentTypeHandler* const type = ComponentTypeHandler::getHandlerFor (*comp))
                        type->addPropertiesToPropertyPanel (comp, document, propsPanel);
            }
        }

    private:
        PMixDocument& document;
        ComponentLayout& layout;
        PropertyPanel propsPanel;
    };
};


#endif   // COMPONENTLAYOUTPANEL_H_INCLUDED
