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
  
    setSize (600, 700);
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
