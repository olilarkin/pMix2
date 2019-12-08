/*
  ==============================================================================

    pMixFileBrowser.cpp
    Author:  Oliver Larkin

  ==============================================================================
*/

#include "pMixFileBrowser.h"

ThumbnailComponent::ThumbnailComponent (AudioFormatManager& formatManager,
                   AudioTransportSource& transportSource_,
                   Slider& slider)
: transportSource (transportSource_),
zoomSlider (slider),
scrollbar (false),
thumbnailCache (5),
thumbnail (512, formatManager, thumbnailCache),
isFollowingTransport (false)
{
  thumbnail.addChangeListener (this);
  
  addAndMakeVisible (scrollbar);
  scrollbar.setRangeLimits (visibleRange);
  scrollbar.setAutoHide (false);
  scrollbar.addListener (this);
  
  currentPositionMarker.setFill (Colours::white.withAlpha (0.85f));
  addAndMakeVisible (currentPositionMarker);
}
  
ThumbnailComponent::~ThumbnailComponent()
{
  scrollbar.removeListener (this);
  thumbnail.removeChangeListener (this);
}

void ThumbnailComponent::setFile (const File& file)
{
  if (! file.isDirectory())
  {
    thumbnail.setSource (new FileInputSource (file));
    const Range<double> newRange (0.0, thumbnail.getTotalLength());
    scrollbar.setRangeLimits (newRange);
    setRange (newRange);
    
    startTimerHz (40);
  }
}

File ThumbnailComponent::getLastDroppedFile() const noexcept
{
  return lastFileDropped;
}

void ThumbnailComponent::setZoomFactor (double amount)
{
  if (thumbnail.getTotalLength() > 0)
  {
    const double newScale = jmax (0.001, thumbnail.getTotalLength() * (1.0 - jlimit (0.0, 0.99, amount)));
    const double timeAtCentre = xToTime (getWidth() / 2.0f);
    setRange (Range<double> (timeAtCentre - newScale * 0.5, timeAtCentre + newScale * 0.5));
  }
}

void ThumbnailComponent::setRange (Range<double> newRange)
{
  visibleRange = newRange;
  scrollbar.setCurrentRange (visibleRange);
  updateCursorPosition();
  repaint();
}

void ThumbnailComponent::setFollowsTransport (bool shouldFollow)
{
  isFollowingTransport = shouldFollow;
}

void ThumbnailComponent::paint (Graphics& g)
{
  g.fillAll (Colours::white);
  g.setColour (Colours::black);
  
  if (thumbnail.getTotalLength() > 0.0)
  {
    Rectangle<int> thumbArea (getLocalBounds());
    thumbArea.removeFromBottom (scrollbar.getHeight() + 4);
    thumbnail.drawChannels (g, thumbArea.reduced (2),
                            visibleRange.getStart(), visibleRange.getEnd(), 1.0f);
  }
  else
  {
    g.setFont (14.0f);
    g.drawFittedText ("(No audio file selected)", getLocalBounds(), Justification::centred, 2);
  }
}

void ThumbnailComponent::resized()
{
  scrollbar.setBounds (getLocalBounds().removeFromBottom (14).reduced (2));
}

void ThumbnailComponent::changeListenerCallback (ChangeBroadcaster*)
{
  // this method is called by the thumbnail when it has changed, so we should repaint it..
  repaint();
}

bool ThumbnailComponent::isInterestedInFileDrag (const StringArray& /*files*/)
{
  return true;
}

void ThumbnailComponent::filesDropped (const StringArray& files, int /*x*/, int /*y*/)
{
  lastFileDropped = File (files[0]);
  sendChangeMessage();
}

void ThumbnailComponent::mouseDown (const MouseEvent& e)
{
  mouseDrag (e);
}

void ThumbnailComponent::mouseDrag (const MouseEvent& e)
{
  if (canMoveTransport())
    transportSource.setPosition (jmax (0.0, xToTime ((float) e.x)));
}

void ThumbnailComponent::mouseUp (const MouseEvent&)
{
  transportSource.start();
}

void ThumbnailComponent::mouseWheelMove (const MouseEvent&, const MouseWheelDetails& wheel)
{
  if (thumbnail.getTotalLength() > 0.0)
  {
    double newStart = visibleRange.getStart() - wheel.deltaX * (visibleRange.getLength()) / 10.0;
    newStart = jlimit (0.0, jmax (0.0, thumbnail.getTotalLength() - (visibleRange.getLength())), newStart);
    
    if (canMoveTransport())
      setRange (Range<double> (newStart, newStart + visibleRange.getLength()));
    
    if (wheel.deltaY != 0.0f)
      zoomSlider.setValue (zoomSlider.getValue() - wheel.deltaY);
    
    repaint();
  }
}

float ThumbnailComponent::timeToX (const double time) const
{
  return getWidth() * (float) ((time - visibleRange.getStart()) / (visibleRange.getLength()));
}

double ThumbnailComponent::xToTime (const float x) const
{
  return (x / getWidth()) * (visibleRange.getLength()) + visibleRange.getStart();
}

bool ThumbnailComponent::canMoveTransport() const noexcept
{
  return ! (isFollowingTransport && transportSource.isPlaying());
}

void ThumbnailComponent::scrollBarMoved (ScrollBar* scrollBarThatHasMoved, double newRangeStart)
{
  if (scrollBarThatHasMoved == &scrollbar)
    if (! (isFollowingTransport && transportSource.isPlaying()))
      setRange (visibleRange.movedToStartAt (newRangeStart));
}

void ThumbnailComponent::timerCallback()
{
  if (canMoveTransport())
    updateCursorPosition();
  else
    setRange (visibleRange.movedToStartAt (transportSource.getCurrentPosition() - (visibleRange.getLength() / 2.0)));
}

void ThumbnailComponent::updateCursorPosition()
{
  currentPositionMarker.setVisible (transportSource.isPlaying() || isMouseButtonDown());
  
  currentPositionMarker.setRectangle (Rectangle<float> (timeToX (transportSource.getCurrentPosition()) - 0.75f, 0,
                                                        1.5f, (float) (getHeight() - scrollbar.getHeight())));
}


FileBrowser::FileBrowser() : thread ("audio file preview")
, directoryList (nullptr, thread)
, fileTreeComp (directoryList)
{
  addAndMakeVisible (zoomLabel);
  zoomLabel.setText ("zoom:", dontSendNotification);
  zoomLabel.setFont (Font (15.00f, Font::plain));
  zoomLabel.setJustificationType (Justification::centredRight);
  zoomLabel.setEditable (false, false, false);
  zoomLabel.setColour (TextEditor::textColourId, Colours::black);
  zoomLabel.setColour (TextEditor::backgroundColourId, Colour (0x00000000));
  
  addAndMakeVisible (followTransportButton);
  followTransportButton.setButtonText ("Follow Transport");
  followTransportButton.addListener (this);
  
  addAndMakeVisible (zoomSlider);
  zoomSlider.setRange (0, 1, 0);
  zoomSlider.setSliderStyle (Slider::LinearHorizontal);
  zoomSlider.setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
  zoomSlider.addListener (this);
  zoomSlider.setSkewFactor (2);
  
  thumbnail = std::make_unique<ThumbnailComponent>(formatManager, transportSource, zoomSlider);
  addAndMakeVisible (*thumbnail);
  thumbnail->addChangeListener (this);
  
  addAndMakeVisible (startStopButton);
  startStopButton.setButtonText ("Play/Stop");
  startStopButton.addListener (this);
  startStopButton.setColour (TextButton::buttonColourId, Colour (0xff79ed7f));
  
  addAndMakeVisible (fileTreeComp);
  
  // audio setup
  formatManager.registerBasicFormats();
  
  directoryList.setDirectory (File::getSpecialLocation (File::userHomeDirectory), true, true);
  thread.startThread (3);
  
  fileTreeComp.setColour (FileTreeComponent::backgroundColourId, Colours::lightgrey.withAlpha (0.6f));
  fileTreeComp.addListener (this);
  
  //deviceManager.addAudioCallback (&audioSourcePlayer);
  audioSourcePlayer.setSource (&transportSource);
  
  setOpaque (true);
}
  
FileBrowser::~FileBrowser()
{
  transportSource.setSource (nullptr);
  audioSourcePlayer.setSource (nullptr);
  
  //deviceManager.removeAudioCallback (&audioSourcePlayer);
  fileTreeComp.removeListener (this);
  thumbnail->removeChangeListener (this);
  followTransportButton.removeListener (this);
  zoomSlider.removeListener (this);
}
  
void FileBrowser::paint (Graphics& g)
{
  //fillTiledBackground (g);
  g.fillAll (Colours::white);
}

void FileBrowser::resized()
{
  Rectangle<int> r (getLocalBounds().reduced (4));
  
  Rectangle<int> controls (r.removeFromBottom (90));
  
  Rectangle<int> zoom (controls.removeFromTop (25));
  zoomLabel.setBounds (zoom.removeFromLeft (50));
  zoomSlider.setBounds (zoom);
  followTransportButton.setBounds (controls.removeFromTop (25));
  startStopButton.setBounds (controls);
  
  r.removeFromBottom (6);
  thumbnail->setBounds (r.removeFromBottom (140));
  r.removeFromBottom (6);
  fileTreeComp.setBounds (r);
}

void FileBrowser::showFile (const File& file)
{
  loadFileIntoTransport (file);
  
  zoomSlider.setValue (0, dontSendNotification);
  thumbnail->setFile (file);
}

void FileBrowser::loadFileIntoTransport (const File& audioFile)
{
  // unload the previous file source and delete it..
  transportSource.stop();
  transportSource.setSource (nullptr);
  currentAudioFileSource = nullptr;
  
  AudioFormatReader* reader = formatManager.createReaderFor (audioFile);
  
  if (reader != nullptr)
  {
    currentAudioFileSource = std::make_unique<AudioFormatReaderSource>(reader, true);
    
    // ..and plug it into our transport source
    transportSource.setSource (currentAudioFileSource.get(),
                               32768,          // tells it to buffer this many samples ahead
                               &thread,         // this is the background thread to use for reading-ahead
                               reader->sampleRate);   // allows for sample rate correction
  }
}

void FileBrowser::selectionChanged()
{
  showFile (fileTreeComp.getSelectedFile());
}

void FileBrowser::sliderValueChanged (Slider* sliderThatWasMoved)
{
  if (sliderThatWasMoved == &zoomSlider)
    thumbnail->setZoomFactor (zoomSlider.getValue());
}

void FileBrowser::buttonClicked (Button* buttonThatWasClicked)
{
  if (buttonThatWasClicked == &startStopButton)
  {
    if (transportSource.isPlaying())
    {
      transportSource.stop();
    }
    else
    {
      transportSource.setPosition (0);
      transportSource.start();
    }
  }
  else if (buttonThatWasClicked == &followTransportButton)
  {
    thumbnail->setFollowsTransport (followTransportButton.getToggleState());
  }
}

void FileBrowser::changeListenerCallback (ChangeBroadcaster* source)
{
  if (source == thumbnail.get())
    showFile (thumbnail->getLastDroppedFile());
}
