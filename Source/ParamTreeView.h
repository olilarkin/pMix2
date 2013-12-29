
#ifndef __PTVHEADER__
#define __PTVHEADER__

class  ParamTreeViewRootItem  : public TreeViewItem
{
public:
  ParamTreeViewRootItem ()
  {
  }

  ~ ParamTreeViewRootItem()
  {
  }

  int getItemWidth() const
  {
    return 100;
  }

  int getItemHeight() const
  {
    return 15;
  }

  String getUniqueName() const
  {
    return String("root");
  }

  bool mightContainSubItems()
  {
    return true;
  }

  void paintItem (Graphics& g, int width, int height)
  {
    // dont paint root
  }

  void itemOpennessChanged (bool isNowOpen)
  {
    /*
    if (isNowOpen)
    {
      if (getNumSubItems() == 0)
      {

      }
    }
    else
    {

    }

     */
  }

private:
};

class  ParamTreeViewParamItem  : public TreeViewItem
{
public:
  ParamTreeViewParamItem (String paramName)
  {
    mName = paramName;
  }

  ~ParamTreeViewParamItem()
  {
  }

  int getItemWidth() const
  {
    return 100;
  }

  int getItemHeight() const
  {
    return 15;
  }

  String getUniqueName() const
  {
    return mName;
  }

  bool mightContainSubItems()
  {
    return false;
  }

  void paintItem (Graphics& g, int width, int height)
  {
    // if this item is selected, fill it with a background colour..
    if (isSelected())
      g.fillAll (Colours::blue.withAlpha (0.3f));

    // use a "colour" attribute in the xml tag for this node to set the text colour..
    g.setColour (Colours::black);

    g.setFont (height * 0.8f);

    // draw the xml element's tag name..
    g.drawText (mName,
                4, 0, width - 4, height,
                Justification::centredLeft, true);

  }

  void itemOpennessChanged (bool isNowOpen)
  {
  }

private:
  String mName;
};

class  ParamTreeViewPluginItem  : public TreeViewItem
{
public:
  ParamTreeViewPluginItem (FilterGraph& graph_, const uint32 filterID_)
    : graph (graph_),
      filterID (filterID_)
  {
  }

  ~ParamTreeViewPluginItem()
  {
  }

  int getItemWidth() const
  {
    return 100;
  }

  int getItemHeight() const
  {
    return 15;
  }

  String getUniqueName() const
  {
    const AudioProcessorGraph::Node::Ptr f (graph.getNodeForId (filterID));
    return f->getProcessor()->getName(); // Unique?
//  return xml.getTagName();
  }

  bool mightContainSubItems()
  {
    return false;
    //return xml.getFirstChildElement() != 0;
  }

  void paintItem (Graphics& g, int width, int height)
  {
    const AudioProcessorGraph::Node::Ptr f (graph.getNodeForId (filterID));

    // if this item is selected, fill it with a background colour..
    if (isSelected())
      g.fillAll (Colours::blue.withAlpha (0.3f));

    // use a "colour" attribute in the xml tag for this node to set the text colour..
    g.setColour (Colours::black);

    g.setFont (height * 0.8f);

    // draw the xml element's tag name..
    g.drawText (f->getProcessor()->getName(),
                4, 0, width - 4, height,
                Justification::centredLeft, true);

  }

  void itemOpennessChanged (bool isNowOpen)
  {
    if (isNowOpen)
    {
      if (getNumSubItems() == 0)
      {
        const AudioProcessorGraph::Node::Ptr f (graph.getNodeForId (filterID));

        for(int i = 0; i < f->getProcessor()->getNumParameters(); i++)
        {
          addSubItem (new ParamTreeViewParamItem  (f->getProcessor()->getParameterName(i)));
        }
      }
    }
    else
    {
    }
  }

private:
  FilterGraph& graph;
  const uint32 filterID;
};

class  ParamTreeView  : public Component,
  public ChangeListener
/*, public DragAndDropContainer */
/*, public ButtonListener*/
{
public:
  ParamTreeView(FilterGraph& graph_)
    : treeView (0)
    , graph (graph_)
  {
    graph.addChangeListener (this);
    setName ("Tree Views");

    rootItem = new  ParamTreeViewRootItem();
    rootItem->setOpen (true);

    showCustomTreeView();
  }

  ~ParamTreeView()
  {
  }

  void paint (Graphics& g)
  {
    g.setColour (Colours::white);

    if (treeView != 0)
      g.drawRect (treeView->getX(), treeView->getY(),
                  treeView->getWidth(), treeView->getHeight());
  }

  void resized()
  {
    if (treeView != 0)
      treeView->setBoundsInset (BorderSize<int> (40, 10, 10, 10));
  }

  void showCustomTreeView()
  {
    treeView = 0;

    addAndMakeVisible (treeView = new TreeView());
    treeView->setRootItemVisible(false);
    treeView->setRootItem (rootItem);
    treeView->setMultiSelectEnabled (true);

    resized();
  }

  void changeListenerCallback (ChangeBroadcaster*)
  {
    rootItem->clearSubItems();
    for(int i = 3; i < graph.getNumFilters(); i++)
    {
      const AudioProcessorGraph::Node::Ptr f (graph.getNode (i));
      addPlugin(f->nodeId);
    }
  }

  void addPlugin(const uint32 filterID_)
  {
    rootItem->addSubItem (new ParamTreeViewPluginItem (graph, filterID_));
  }

  //void buttonClicked (Button*) {}

  juce_UseDebuggingNewOperator

private:
  FilterGraph& graph;
  ScopedPointer <TreeViewItem> rootItem;
  ScopedPointer <TreeView> treeView;

};


#endif
