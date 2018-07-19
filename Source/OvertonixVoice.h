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
  void generateWavetable();
private:
  template <typename FloatType>
  void processBlock(AudioBuffer<FloatType>&, int, int);
  double level = 0.0, tailOff = 0.0, currentSampleDelta = 0.0,currentSampleIndex = 0.0;
    StringRef strs[9] = { 
        StringRef("overtoneLevel1"),
        StringRef("overtoneLevel2"),
        StringRef("overtoneLevel3"),
        StringRef("overtoneLevel4"),
        StringRef("overtoneLevel5"),
        StringRef("overtoneLevel6"),
        StringRef("overtoneLevel7"),
        StringRef("highEndSlope"),
        StringRef("highEndLevel")
    };
  double wavetable[128];
  double newValues[9];
  AudioProcessorValueTreeState* params;
};
