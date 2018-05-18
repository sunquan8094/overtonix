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
    tailOff = 0.0;
}

void OvertonixVoice::stopNote(float velocity, bool allowTailOff) {
    if (allowTailOff) {
        if (tailOff == 0.0) tailOff = 1.0;
    }
    else {
        clearCurrentNote();
        angleDelta = 0.0;
    }
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
    auto current = static_cast<FloatType> (addSineWaves() * ((tailOff > 0.0) ? tailOff : 1.0));
          
      for (int i = out.getNumChannels(); --i >= 0;) out.addSample(i, start, current);
          
      currentAngle += angleDelta;
      ++start;
        
        if (tailOff > 0.0) {
            tailOff *= 0.99;
        
            if (tailOff <= 0.005) {
                angleDelta = 0.0;
                break;
            }
        }
    }
  }
}

double OvertonixVoice::addSineWaves() {
    double retval = 0.0;
    float levels[7] = {
      *(params->getRawParameterValue(StringRef("overtoneLevel1"))),
      *(params->getRawParameterValue(StringRef("overtoneLevel2"))),
      *(params->getRawParameterValue(StringRef("overtoneLevel3"))),
      *(params->getRawParameterValue(StringRef("overtoneLevel4"))),
      *(params->getRawParameterValue(StringRef("overtoneLevel5"))),
      *(params->getRawParameterValue(StringRef("overtoneLevel6"))),
      *(params->getRawParameterValue(StringRef("overtoneLevel7")))
    }, highEndSlope = *(params->getRawParameterValue(StringRef("highEndSlope"))), 
    highEndLevel = *(params->getRawParameterValue(StringRef("highEndLevel")));
    retval += std::sin(currentAngle) * 100.0;
    for (int o = 0; o < 7; o++) {
      retval += std::sin((o + 2) * currentAngle) * levels[o];
    }
    for (int k = 9; k < 16; k++) {
        retval += std::sin(k * currentAngle) * highEndLevel * pow(highEndSlope, 8 - k);
    }
    retval *= level * 0.01;
    return retval;
}

void OvertonixVoice::pitchWheelMoved (int newPitchWheelValue) {

}

void OvertonixVoice::controllerMoved (int controllerNumber, int newControllerValue) {

}
