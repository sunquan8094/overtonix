#include "OvertonixVoice.h"
#include "OvertonixSound.h"

#define NUM_SAMPLES 1024

OvertonixVoice::OvertonixVoice(AudioProcessorValueTreeState& args) : params(&args) {
  generateSinTable(NUM_SAMPLES);
  
}

void OvertonixVoice::generateSinTable(int samples) {
  for (int x = 0; x < 20; x++) {
    for (int y = 0; y < samples; y++) {
      sinTable[x][y] = std::sin((double)(x) * ((double)y * (2.0 * double_Pi / (double)samples)));
    }
  }
}

bool OvertonixVoice::canPlaySound(SynthesiserSound* sound) {
  return dynamic_cast<OvertonixSound*> (sound) != nullptr;
}

void OvertonixVoice::startNote(int note, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) {
    level = velocity * 0.001;
    double cycHz = MidiMessage::getMidiNoteInHertz(note);
    currentSampleIndex = 0.0;
    currentSampleDelta = cycHz * NUM_SAMPLES / getSampleRate();
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
      if (currentSampleIndex > NUM_SAMPLES) currentSampleIndex -= NUM_SAMPLES;
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
    double retval = 0.0;
    float highEndSlope = *(params->getRawParameterValue(strs[7])), highEndLevel = *(params->getRawParameterValue(strs[8]));
    for (int p = 0; p < NUM_SAMPLES; p++) {
        retval = sinTable[1][p] * 100.0;
        
        for (int o = 2; o < 9; o++) {
          double level = *(params->getRawParameterValue(strs[o - 2]));
          retval += sinTable[o][p] * level;
        }
        
        for (int k = 9; k < 20; k++) {
            retval += sinTable[k][p] * (highEndLevel) * pow((float)5 - highEndSlope, (float)8 - k);
        }
        wavetable[p] = retval;
    }
}

void OvertonixVoice::pitchWheelMoved (int newPitchWheelValue) {

}

void OvertonixVoice::controllerMoved (int controllerNumber, int newControllerValue) {

}
