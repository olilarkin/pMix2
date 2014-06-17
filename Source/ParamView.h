class ProcessorParameterPropertyComp :  public PropertyComponent,
                                        private AudioProcessorListener,
                                        private Timer
{
public:
  ProcessorParameterPropertyComp (const String& name, AudioProcessor& p, const int index_)
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
  
  ~ProcessorParameterPropertyComp()
  {
    owner.removeListener (this);
  }
  
  void refresh()
  {
    paramHasChanged = false;
    slider.setValueEx (owner.getParameter (index));
  }
  
  void audioProcessorChanged (AudioProcessor*)  {}
  
  void audioProcessorParameterChanged (AudioProcessor*, int parameterIndex, float)
  {
    if (parameterIndex == index)
      paramHasChanged = true;
  }
  
  void timerCallback() override
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
  
private:
  class ParamSlider  : public Slider
  {
  public:
    ParamSlider (AudioProcessor& p, const int index_)
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
    
    void setValueEx(float value)
    {
      if (getThumbBeingDragged() == -1) 
      {
        setValue(value, dontSendNotification);
      }
    }
    
    void valueChanged()
    {
      const float newVal = (float) getValue();
      
      if (owner.getParameter (index) != newVal)
        owner.setParameterNotifyingHost (index, newVal);
    }
    
    String getTextFromValue (double /*value*/)
    {
      return owner.getParameterText (index);
    }
    
  private:
    AudioProcessor& owner;
    const int index;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParamSlider)
  };
  
  AudioProcessor& owner;
  const int index;
  bool volatile paramHasChanged;
  ParamSlider slider;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProcessorParameterPropertyComp)
};

class ParamView : public Component
                , public ChangeListener
{
public:  
  ParamView(FilterGraph& graph_)
  : graph (graph_)
  {
    graph.addChangeListener (this);
    
    setOpaque (true);
    addAndMakeVisible (&panel);
  }
  
  ~ParamView()
  {
    graph.removeChangeListener (this);
  }
  
  void paint (Graphics& g) override
  {
    g.fillAll (Colour::greyLevel (0.8f));
  }
  
  void resized() override
  {
    panel.setBounds (getLocalBounds().reduced (4));
  }
  
  void changeListenerCallback (ChangeBroadcaster* source)
  {
    for (int i = graph.getNumFilters(); --i >= 0;)
    {
      const AudioProcessorGraph::Node::Ptr f (graph.getNode (i));
      
      //todo check existing nodes
      
      if   (f->getProcessor()->getName() != "Audio Input"
         && f->getProcessor()->getName() != "Audio Output" 
         && f->getProcessor()->getName() != "Midi Input" 
         && f->getProcessor()->getName() != "Midi Output") 
      {
        if (!sectionNodes.contains(f)) 
        {
          sectionNodes.add(f);
          addEditor(f->getProcessor());
        }
      }
    }
  }
  
  void addEditor(AudioProcessor* p)
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
  
private:
  FilterGraph& graph;
  Array<AudioProcessorGraph::Node::Ptr> sectionNodes;
  PropertyPanel panel;
};