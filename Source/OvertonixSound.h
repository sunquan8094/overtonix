#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class OvertonixSound : public SynthesiserSound {
public:
  OvertonixSound() {}
  bool appliesToNote(int) override { return true; }
  bool appliesToChannel(int) override {return true; }
};
