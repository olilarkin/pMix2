/*
  ==============================================================================

    pMixFileBrowser.h
    Author:  Oliver Larkin

  ==============================================================================
*/

#ifndef FileBrowser_H_INCLUDED
#define FileBrowser_H_INCLUDED

#include "JuceHeader.h"

class ThumbnailComponent  : public Component,
                            public ChangeListener,
                            public FileDragAndDropTarget,
                            public ChangeBroadcaster,
                            private ScrollBar::Listener,
                            private Timer
{
public:
  ThumbnailComponent (AudioFormatManager& formatManager, AudioTransportSource& transportSource_, Slider& slider);
  ~ThumbnailComponent();
  void setFile (const File& file);
  File getLastDroppedFile() const noexcept;
  void setZoomFactor (double amount);
  void setRange (Range<double> newRange);
  
  void setFollowsTransport (bool shouldFollow);
  void paint (Graphics& g) override;
  
  void resized() override;
  void changeListenerCallback (ChangeBroadcaster*) override;
  bool isInterestedInFileDrag (const StringArray& /*files*/) override;
  void filesDropped (const StringArray& files, int /*x*/, int /*y*/) override;
  void mouseDown (const MouseEvent& e) override;
  void mouseDrag (const MouseEvent& e) override;
  void mouseUp (const MouseEvent&) override;
  
  void mouseWheelMove (const MouseEvent&, const MouseWheelDetails& wheel) override;
  
private:
  AudioTransportSource& transportSource;
  Slider& zoomSlider;
  ScrollBar scrollbar;
  
  AudioThumbnailCache thumbnailCache;
  AudioThumbnail thumbnail;
  Range<double> visibleRange;
  bool isFollowingTransport;
  File lastFileDropped;
  
  DrawableRectangle currentPositionMarker;
  
  float timeToX (const double time) const;
  double xToTime (const float x) const;
  bool canMoveTransport() const noexcept;
  void scrollBarMoved (ScrollBar* scrollBarThatHasMoved, double newRangeStart) override;
  void timerCallback() override;
  void updateCursorPosition();
};

class FileBrowser : public Component,
                        private FileBrowserListener,
                        private Button::Listener,
                        private Slider::Listener,
                        private ChangeListener
{
public:
  FileBrowser();
  ~FileBrowser();
  
  void paint (Graphics& g) override;
  void resized() override;
  
private:
  //AudioDeviceManager& deviceManager;
  AudioFormatManager formatManager;
  TimeSliceThread thread;
  DirectoryContentsList directoryList;
  
  AudioSourcePlayer audioSourcePlayer;
  AudioTransportSource transportSource;
  ScopedPointer<AudioFormatReaderSource> currentAudioFileSource;
  
  ScopedPointer<ThumbnailComponent> thumbnail;
  Label zoomLabel;
  Slider zoomSlider;
  ToggleButton followTransportButton;
  TextButton startStopButton;
  FileTreeComponent fileTreeComp;
  
  void showFile (const File& file);
  
  void loadFileIntoTransport (const File& audioFile);
  
  void selectionChanged() override;
  
  void fileClicked (const File&, const MouseEvent&) override     {}
  void fileDoubleClicked (const File&) override            {}
  void browserRootChanged (const File&) override           {}

  void sliderValueChanged (Slider* sliderThatWasMoved) override;
  void buttonClicked (Button* buttonThatWasClicked) override;
  void changeListenerCallback (ChangeBroadcaster* source) override;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FileBrowser);
};


#endif  // FileBrowser_H_INCLUDED
