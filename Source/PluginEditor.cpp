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
        attachments[o].reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "overtoneLevel" + std::to_string(o + 1), sliders[o]));
        labels[o].setText("Overtone " + std::to_string(o + 1) + " Level", dontSendNotification);
        sliders[o].setTextValueSuffix("%");
    }
        
            
    attachments[7].reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "highEndLevel", sliders[7]));
    labels[7].setText("High End Level", dontSendNotification);
    sliders[7].setTextValueSuffix("%");
      
    attachments[8].reset(new juce::AudioProcessorValueTreeState::SliderAttachment(valueTreeState, "highEndSlope", sliders[8]));
            labels[8].setText("High End Slope", dontSendNotification);
      
      for (int o = 0; o < 9; o++) {
        labels[o].attachToComponent(&(sliders[o]), false);
      
        sliders[o].setBounds(50, 75 * o + 50, 500, 70);
        sliders[o].addListener(this);
        
        addAndMakeVisible(sliders[o]);
        addAndMakeVisible(labels[o]);
      }
      
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

void OvertonixAudioProcessorEditor::sliderValueChanged(Slider *slider) {
  processor.updateValue();
}
