
#ifndef _INTERPOLATIONSPACE_H_
#define _INTERPOLATIONSPACE_H_

#include "JuceHeader.h"
#include "pMixGraphEditor.h"
#include "pMixAudioEngine.h"
#include "pMixInterpolationSpaceLayout.h"
#include "pMixInterpolationSpaceCrosshairs.h"

class InterpolationSpace : public Component
{
public:
  InterpolationSpace (PMixAudioEngine& audioEngine, GraphEditor& graphEditor);
  ~InterpolationSpace ();
  void resized ();
  void paint (Graphics& g);
  
private:
  ScopedPointer<PMixInterpolationSpaceLayout> layout;
  ScopedPointer<pMixInterpolationSpaceCrossHairs> crosshairs;
};

#endif //_INTERPOLATIONSPACE_H_ 