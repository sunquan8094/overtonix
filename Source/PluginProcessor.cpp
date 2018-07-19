/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "OvertonixSound.h"
#include "OvertonixVoice.h"

//==============================================================================
OvertonixAudioProcessor::OvertonixAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ), parameters(*this, nullptr)
#endif
{
  
    for (int o = 0; o < 7; o++)  {
        parameters.createAndAddParameter("overtoneLevel" + std::to_string(o + 1), "Overtone " + std::to_string(o + 1) + " Level", String(), NormalisableRange<float>(0.0f, 100.0f, 1.0f), floorf(powf(2, -1 * o) * 100.f), nullptr, nullptr);
    }    
    parameters.createAndAddParameter("highEndLevel", "High End Level", String(), NormalisableRange<float>(0.0f, 100.0f, 1.0f), 100.f, nullptr, nullptr);
    
    parameters.createAndAddParameter("highEndSlope", "High End Slope", String(), NormalisableRange<float>(1.20f, 2.75f, 0.05f), 1.5f, nullptr, nullptr);
  
  parameters.state = ValueTree(Identifier("Overtonix"));
  
  for (int j = 0; j < 8; j++) synth.addVoice(new OvertonixVoice(parameters));
    synth.addSound(new OvertonixSound());
}

OvertonixAudioProcessor::~OvertonixAudioProcessor()
{
}

//==============================================================================
const String OvertonixAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool OvertonixAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool OvertonixAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool OvertonixAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double OvertonixAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int OvertonixAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int OvertonixAudioProcessor::getCurrentProgram()
{
    return 0;
}

void OvertonixAudioProcessor::setCurrentProgram (int index)
{
}

const String OvertonixAudioProcessor::getProgramName (int index)
{
    return {};
}

void OvertonixAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void OvertonixAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    synth.setCurrentPlaybackSampleRate(sampleRate);
    keyboardState.reset();
}

void OvertonixAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OvertonixAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void OvertonixAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
  process(buffer, midiMessages);
}

void OvertonixAudioProcessor::processBlock(AudioBuffer<double>& buffer, MidiBuffer& midiMessages)
{
  process(buffer, midiMessages);
}

void OvertonixAudioProcessor::updateValue() {
    for (int v = 0; v < 8; v++) {
        OvertonixVoice* voice = (OvertonixVoice*) (synth.getVoice(v));
        voice->generateWavetable();
    }
}

template <typename FloatType>
void OvertonixAudioProcessor::process(AudioBuffer<FloatType>& buffer,
                                      MidiBuffer& midiMessages) {
  int num = buffer.getNumSamples();
  for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i) buffer.clear(i, 0, num);
  keyboardState.processNextMidiBuffer(midiMessages, 0, num, true);
  synth.renderNextBlock(buffer, midiMessages, 0, num);
}

//==============================================================================
bool OvertonixAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* OvertonixAudioProcessor::createEditor()
{
    return new OvertonixAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void OvertonixAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
  auto state = parameters.copyState();
  std::unique_ptr<XmlElement> xml (state.createXml());
  copyXmlToBinary (*xml, destData);
}

void OvertonixAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
      if (xmlState->hasTagName (parameters.state.getType()))
        parameters.replaceState (ValueTree::fromXml (*xmlState));
  
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OvertonixAudioProcessor();
}
