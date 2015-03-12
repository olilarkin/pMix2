
#ifndef _INTERPOLATIONSPACE_H_
#define _INTERPOLATIONSPACE_H_

#include "JuceHeader.h"
#include "pMixAudioEngine.h"
#include "pMixInterpolationSpaceLayout.h"
#include "pMixInterpolationSpaceCrosshairs.h"

class InterpolationSpace : public Component
{
public:
  InterpolationSpace (PMixAudioEngine& audioEngine);
  ~InterpolationSpace ();
  void resized ();
  void paint (Graphics& g);
private:
  ScopedPointer<PMixInterpolationSpaceLayout> layout;
};

#endif //_INTERPOLATIONSPACE_H_ 