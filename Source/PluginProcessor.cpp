/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PaperDelayAudioProcessor::PaperDelayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
apvts(*this, nullptr, "Parameters", createParameterLayout())
#endif
{
    apvts.addParameterListener("Time", this);
}

PaperDelayAudioProcessor::~PaperDelayAudioProcessor()
{
    apvts.removeParameterListener("Time", this);
}

//==============================================================================
const juce::String PaperDelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PaperDelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PaperDelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PaperDelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PaperDelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PaperDelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PaperDelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PaperDelayAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PaperDelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void PaperDelayAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PaperDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumInputChannels();
    spec.maximumBlockSize = samplesPerBlock;
    
    delay.reset();
    delay.setMaximumDelayInSamples(sampleRate * 5);
    delay.setDelay(calculateTimeToSamples(apvts.getRawParameterValue("Time")->load()));
    delay.prepare(spec);
}

void PaperDelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PaperDelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void PaperDelayAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    auto feedback = apvts.getRawParameterValue("Feedback")->load();
    
    for (auto channel = 0; channel < getTotalNumOutputChannels(); ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        
        for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            auto output = delay.popSample(channel);
            delay.pushSample(channel, channelData[sample] + feedback * output);
            channelData[sample] = output;
        }
    }
}

//==============================================================================
bool PaperDelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PaperDelayAudioProcessor::createEditor()
{
//    return new PaperDelayAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void PaperDelayAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PaperDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PaperDelayAudioProcessor();
}

void PaperDelayAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    delay.setDelay(calculateTimeToSamples(apvts.getRawParameterValue("Time")->load()));
}

juce::AudioProcessorValueTreeState::ParameterLayout PaperDelayAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Time", 1), "Time", juce::NormalisableRange<float>(1.f, 5000.f, 1.f, 1.f), 500.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Feedback", 2), "Feedback", juce::NormalisableRange<float>(0.f, 1.f, 0.01f, 1.f), 0.2f));
    
    return layout;
}

float PaperDelayAudioProcessor::calculateTimeToSamples(float delayInMilliseconds)
{
    const auto sampleRate = getSampleRate();
    
    return static_cast<float>(delayInMilliseconds * (sampleRate / 1000));
}
