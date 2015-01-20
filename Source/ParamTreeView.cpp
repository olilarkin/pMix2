#include "ParamTreeView.h"

ParamTreeViewRootItem::ParamTreeViewRootItem ()
{
}

ParamTreeViewRootItem::~ParamTreeViewRootItem()
{
}

int ParamTreeViewRootItem::getItemWidth() const
{
  return 100;
}

int ParamTreeViewRootItem::getItemHeight() const
{
  return 15;
}

String ParamTreeViewRootItem::getUniqueName() const
{
  return String("root");
}

bool ParamTreeViewRootItem::mightContainSubItems()
{
  return true;
}

void ParamTreeViewRootItem::paintItem (Graphics& g, int width, int height)
{
  // dont paint root
}

void ParamTreeViewRootItem::itemOpennessChanged (bool isNowOpen)
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

ParamTreeViewParamItem::ParamTreeViewParamItem (String paramName)
{
  mName = paramName;
}

ParamTreeViewParamItem::~ParamTreeViewParamItem()
{
}

int ParamTreeViewParamItem::getItemWidth() const
{
  return 100;
}

int ParamTreeViewParamItem::getItemHeight() const
{
  return 15;
}

String ParamTreeViewParamItem::getUniqueName() const
{
  return mName;
}

bool ParamTreeViewParamItem::mightContainSubItems()
{
  return false;
}

void ParamTreeViewParamItem::paintItem (Graphics& g, int width, int height)
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

void ParamTreeViewParamItem::itemOpennessChanged (bool isNowOpen)
{
}
  
ParamTreeViewPluginItem::ParamTreeViewPluginItem (PMixDocument& graph_, const uint32 filterID_)
: graph (graph_),
filterID (filterID_)
{
}

ParamTreeViewPluginItem::~ParamTreeViewPluginItem()
{
}

int ParamTreeViewPluginItem::getItemWidth() const
{
  return 100;
}

int ParamTreeViewPluginItem::getItemHeight() const
{
  return 15;
}

String ParamTreeViewPluginItem::getUniqueName() const
{
  const AudioProcessorGraph::Node::Ptr f (graph.getNodeForId (filterID));
  return f->getProcessor()->getName(); // Unique?
  //  return xml.getTagName();
}

bool ParamTreeViewPluginItem::mightContainSubItems()
{
  return false;
  //return xml.getFirstChildElement() != 0;
}

void ParamTreeViewPluginItem::paintItem (Graphics& g, int width, int height)
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

void ParamTreeViewPluginItem::itemOpennessChanged (bool isNowOpen)
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

ParamTreeView::ParamTreeView(PMixDocument& graph_)
: graph (graph_)
, treeView (0)
{
  graph.addChangeListener (this);
  setName ("Tree Views");
  
  rootItem = new  ParamTreeViewRootItem();
  rootItem->setOpen (true);
  
  showCustomTreeView();
}

ParamTreeView::~ParamTreeView()
{
}

void ParamTreeView::paint (Graphics& g)
{
  g.setColour (Colours::white);
  
  if (treeView != 0)
    g.drawRect (treeView->getX(), treeView->getY(),
                treeView->getWidth(), treeView->getHeight());
}

void ParamTreeView::resized()
{
  if (treeView != 0)
    treeView->setBoundsInset (BorderSize<int> (40, 10, 10, 10));
}

void ParamTreeView::showCustomTreeView()
{
  treeView = 0;
  
  addAndMakeVisible (treeView = new TreeView());
  treeView->setRootItemVisible(false);
  treeView->setRootItem (rootItem);
  treeView->setMultiSelectEnabled (true);
  
  resized();
}

void ParamTreeView::changeListenerCallback (ChangeBroadcaster*)
{
  rootItem->clearSubItems();
  for(int i = 3; i < graph.getNumFilters(); i++)
  {
    const AudioProcessorGraph::Node::Ptr f (graph.getNode (i));
    addPlugin(f->nodeId);
  }
}

void ParamTreeView::addPlugin(const uint32 filterID_)
{
  rootItem->addSubItem (new ParamTreeViewPluginItem (graph, filterID_));
}

//void buttonClicked (Button*) {}