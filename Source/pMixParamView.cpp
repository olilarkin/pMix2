#include "pMixParamView.h"

ParamSlider::ParamSlider (AudioProcessor& p, const int index_)
: owner (p),
index (index_)
{
  //      int steps = owner.getParameterNumSteps(index);
  
  float div = 0.;
  
  //      if (steps > 0) {
  //        div = 1.f/steps;
  //      }
  
  setRange (0.0, 1.0, div);
  setSliderStyle (Slider::LinearHorizontal);
  setTextBoxIsEditable (false);
  setScrollWheelEnabled (false);
}

void ParamSlider::setValueEx(float value)
{
  if (getThumbBeingDragged() == -1) 
  {
    setValue(value, dontSendNotification);
  }
}
  
void ParamSlider::valueChanged()
{
  const float newVal = (float) getValue();
  
  if (owner.getParameter (index) != newVal)
    owner.setParameterNotifyingHost (index, newVal);
}
  
String ParamSlider::getTextFromValue (double /*value*/)
{
  return owner.getParameterText (index);
}

ProcessorParameterPropertyComp::ProcessorParameterPropertyComp(const String& name, AudioProcessor& p, const int index_)
: PropertyComponent (name, 20),
owner (p),
index (index_),
paramHasChanged (false),
slider (p, index_)
{
  startTimer (100);
  addAndMakeVisible (slider);
  owner.addListener (this);
}
  
ProcessorParameterPropertyComp::~ProcessorParameterPropertyComp()
{
//  owner.removeListener (this);
}

void ProcessorParameterPropertyComp::refresh()
{
  paramHasChanged = false;
  slider.setValueEx (owner.getParameter (index));
}

void ProcessorParameterPropertyComp::audioProcessorChanged (AudioProcessor*)  {}

void ProcessorParameterPropertyComp::audioProcessorParameterChanged (AudioProcessor*, int parameterIndex, float)
{
  if (parameterIndex == index)
    paramHasChanged = true;
}

void ProcessorParameterPropertyComp::timerCallback()
{
  if (paramHasChanged)
  {
    refresh();
    startTimer (1000 / 50);
  }
  else
  {
    startTimer (jmin (1000 / 4, getTimerInterval() + 10));
  }
}

ParamView::ParamView(PMixDocument& graph_)
: graph (graph_)
{
  graph.addChangeListener (this);
  
  setOpaque (true);
  addAndMakeVisible (&panel);
}

ParamView::~ParamView()
{
  graph.removeChangeListener (this);
}

void ParamView::paint (Graphics& g)
{
  g.fillAll (Colour::greyLevel (0.8f));
}

void ParamView::resized()
{
  panel.setBounds (getLocalBounds().reduced (4));
}
  
void ParamView::changeListenerCallback (ChangeBroadcaster* source)
{
  bool deletedNodes = false;
  
  for (int i = 0; i < sectionNodes.size(); i++) 
  {
    const AudioProcessorGraph::Node::Ptr f (graph.getNodeForId(sectionNodes[i]));
    
    if (f == nullptr) 
    {
      deletedNodes = true;
    }
  }
  
  if (deletedNodes) 
  {
    sectionNodes.clear();
    panel.clear();
  }
  
  for (int i = graph.getNumFilters(); --i >= 0;)
  {
    const AudioProcessorGraph::Node::Ptr f (graph.getNode (i));
    
    if   (f->getProcessor()->getName() != "Audio Input"
          && f->getProcessor()->getName() != "Audio Output" 
          && f->getProcessor()->getName() != "Midi Input" 
          && f->getProcessor()->getName() != "Midi Output") 
    {
      if (!sectionNodes.contains(f->nodeId)) 
      {
        sectionNodes.add(f->nodeId);
        addEditor(f->getProcessor());
      }
    }
  }
}

void ParamView::addEditor(AudioProcessor* p)
{
  Array <PropertyComponent*> params;
  
  const int numParams = p->getNumParameters();
  int totalHeight = 0;
  
  for (int i = 0; i < numParams; ++i)
  {
    String name (p->getParameterName (i));
    if (name.trim().isEmpty())
      name = "Unnamed";
    
    ProcessorParameterPropertyComp* const pc = new ProcessorParameterPropertyComp (name, *p, i);
    params.add (pc);
    totalHeight += pc->getPreferredHeight();
  }
  
  panel.addSection (p->getName(), params, false);
}