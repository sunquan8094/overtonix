#include "OvertonixVoice.h"
#include "OvertonixSound.h"

OvertonixVoice::OvertonixVoice(AudioProcessorValueTreeState& args) : params(&args) {

}

bool OvertonixVoice::canPlaySound(SynthesiserSound* sound) {
  return dynamic_cast<OvertonixSound*> (sound) != nullptr;
}

void OvertonixVoice::startNote(int note, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) {
    level = velocity * 0.001;
    double cycHz = MidiMessage::getMidiNoteInHertz(note);
    currentSampleIndex = 0.0;
    currentSampleDelta = cycHz * 128.0 / getSampleRate();
    generateWavetable();
    tailOff = 0.0;
}



void OvertonixVoice::stopNote(float velocity, bool allowTailOff) {
    if (allowTailOff) {
        if (tailOff == 0.0) tailOff = 1.0;
    }
    else {
        clearCurrentNote();
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
    while (--num >= 0) {
      auto current = static_cast<FloatType> (wavetable[(int)ceil(currentSampleIndex)] * level * ((tailOff > 0.0) ? tailOff : 1.0));
          
      for (int i = out.getNumChannels(); --i >= 0;) out.addSample(i, start, current);
          
      currentSampleIndex += currentSampleDelta;
      if (currentSampleIndex > 128) currentSampleIndex -= 128;
      ++start;
        
        if (tailOff > 0.0) {
            tailOff *= 0.99;
        
            if (tailOff <= 0.005) {
                break;
            }
        }
    }
}

void OvertonixVoice::generateWavetable() {
    double retval = 0.0, angle = 0.0, delta = 2.0 * double_Pi / 128.0;
    float highEndSlope = *(params->getRawParameterValue(strs[7])), highEndLevel = *(params->getRawParameterValue(strs[8]));
    for (int p = 0; p < 128; p++) {
        retval = std::sin(angle) * 100.0;
        
        for (int o = 0; o < 7; o++) {
          double level = *(params->getRawParameterValue(strs[o]));
          retval += std::sin((o + 2) * angle) * level;
        }
        
        for (int k = 9; k < 20; k++) {
            retval += std::sin(k * angle) * (highEndLevel) * pow(5 - highEndSlope, 8 - k);
        }
        wavetable[p] = retval;
        angle += delta;
    }
}

void OvertonixVoice::pitchWheelMoved (int newPitchWheelValue) {

}

void OvertonixVoice::controllerMoved (int controllerNumber, int newControllerValue) {

}
