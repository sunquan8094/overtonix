#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class OvertonixVoice : public SynthesiserVoice {
public:
  OvertonixVoice(AudioProcessorValueTreeState&);
  bool canPlaySound(SynthesiserSound*) override;
  void startNote(int, float, SynthesiserSound*, int) override;
  void stopNote(float, bool) override;
  void renderNextBlock(AudioBuffer<float>&, int, int) override;
  void renderNextBlock(AudioBuffer<double>&, int, int) override;
  void pitchWheelMoved(int) override;
  void controllerMoved(int, int) override;
private:
  template <typename FloatType>
  void processBlock(AudioBuffer<FloatType>&, int, int);
  double currentAngle = 0.0, angleDelta = 0.0, level = 0.0;
  AudioProcessorValueTreeState* params;
  double addSineWaves();
};
