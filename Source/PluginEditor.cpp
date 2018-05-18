/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OvertonixAudioProcessorEditor::OvertonixAudioProcessorEditor (OvertonixAudioProcessor& p, AudioProcessorValueTreeState& vts)
: AudioProcessorEditor (&p), processor (p), valueTreeState(vts), keyboardComponent(p.keyboardState, MidiKeyboardComponent::horizontalKeyboard)
{
    for (int o = 0; o < 7; o++) {
      nthOvertoneLevelAttachment[o].reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "overtoneLevel" + std::to_string(o + 1), nthOvertoneLevelSlider[o]));
      nthOvertoneLevelSlider[o].setTextValueSuffix("%");
      nthOvertoneLevelLabel[o].setText("Overtone " + std::to_string(o + 1) + " Level", dontSendNotification);
      nthOvertoneLevelLabel[o].attachToComponent(&nthOvertoneLevelSlider[o], false);
      
      nthOvertoneLevelSlider[o].setBounds(50, 75 * o + 50, 500, 70);
      
      addAndMakeVisible(nthOvertoneLevelSlider[o]);
      addAndMakeVisible(nthOvertoneLevelLabel[o]);
    }
  
    highEndLevelAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "highEndLevel", highEndLevelSlider));
    highEndLevelSlider.setTextValueSuffix("%");
    highEndLevelLabel.setText("High End Level", dontSendNotification);
    highEndLevelLabel.attachToComponent(&highEndLevelSlider, false);
    highEndLevelSlider.setBounds(50, 575, 500, 70);
    addAndMakeVisible(highEndLevelSlider);
    addAndMakeVisible(highEndLevelLabel);
    
    highEndSlopeAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "highEndSlope", highEndSlopeSlider));
    highEndSlopeLabel.setText("High End Slope", dontSendNotification);
    highEndSlopeLabel.attachToComponent(&highEndSlopeSlider, false);
    highEndSlopeSlider.setBounds(50, 650, 500, 70);
    addAndMakeVisible(highEndSlopeSlider);
    addAndMakeVisible(highEndSlopeLabel);
    
    setSize (600, 735);
}

OvertonixAudioProcessorEditor::~OvertonixAudioProcessorEditor()
{
}

//==============================================================================
void OvertonixAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
}

void OvertonixAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
