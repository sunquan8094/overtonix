#include "OvertonixVoice.h"
#include "OvertonixSound.h"

OvertonixVoice::OvertonixVoice(AudioProcessorValueTreeState& args) : params(&args) {

}

bool OvertonixVoice::canPlaySound(SynthesiserSound* sound) {
  return dynamic_cast<OvertonixSound*> (sound) != nullptr;
}

void OvertonixVoice::startNote(int note, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) {
  currentAngle = 0.0;
  level = velocity * 0.25;
  double cycHz = MidiMessage::getMidiNoteInHertz(note);
  double cycSamp = cycHz / getSampleRate();
  angleDelta = cycSamp * 2.0 * double_Pi;
}

void OvertonixVoice::stopNote(float velocity, bool allowTailOff) {
  clearCurrentNote();
  angleDelta = 0.0;
}

void OvertonixVoice::renderNextBlock(AudioBuffer<float>& out, int start, int num) {
  processBlock(out, start, num);
}

void OvertonixVoice::renderNextBlock(AudioBuffer<double>& out, int start, int num) {
  processBlock(out, start, num);
}

template <typename FloatType>
void OvertonixVoice::processBlock(AudioBuffer<FloatType>& out, int start, int num) {
  if (angleDelta != 0.0) {
    while (--num >= 0) {
      auto current = static_cast<FloatType> (addSineWaves());
          
      for (int i = out.getNumChannels(); --i >= 0;) out.addSample(i, start, current);
          
      currentAngle += angleDelta;
      ++start;
    }
  }
}

double OvertonixVoice::addSineWaves() {
  double retval = 0.0;
  for (int o = 0; o < 7; o++) {
    float nthLevel = *(params->getRawParameterValue(StringRef("overtoneLevel" + std::to_string(o + 1))));
    retval += std::sin((o + 2) * currentAngle) * level * nthLevel * .01;
  }
  return retval;
}

void OvertonixVoice::pitchWheelMoved (int newPitchWheelValue) {

}

void OvertonixVoice::controllerMoved (int controllerNumber, int newControllerValue) {

}
