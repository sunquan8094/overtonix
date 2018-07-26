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
      
      for (int o = 0; o < 8; o++) {
        labels[o].attachToComponent(&(sliders[o]), false);
        sliders[o].setSliderStyle(Slider::SliderStyle::Rotary);
        sliders[o].setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 75, 30);
        sliders[o].setBounds((o % 4 + 1) * 100 - 75, ((int)(o / 4) + 1) * 120 - 75, 100, 100);
        sliders[o].addListener(this);
        
        addAndMakeVisible(sliders[o]);
        addAndMakeVisible(labels[o]);
      }
  
      labels[8].attachToComponent(&(sliders[8]), false);
      sliders[8].setSliderStyle(Slider::SliderStyle::LinearHorizontal);
      sliders[8].setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 100, 30);
      sliders[8].setBounds(25, 285, 400, 100);
      sliders[8].addListener(this);
    
      addAndMakeVisible(sliders[8]);
      addAndMakeVisible(labels[8]);
  
    setSize (450, 400);
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
