/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class OvertonixAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    OvertonixAudioProcessorEditor (OvertonixAudioProcessor&, AudioProcessorValueTreeState&);
    ~OvertonixAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    OvertonixAudioProcessor& processor;
  
    AudioProcessorValueTreeState& valueTreeState;
  
    Slider nthOvertoneLevelSlider[7];
    Label nthOvertoneLevelLabel[7];
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> nthOvertoneLevelAttachment[7];
  
    MidiKeyboardComponent keyboardComponent;
  
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OvertonixAudioProcessorEditor)
};
