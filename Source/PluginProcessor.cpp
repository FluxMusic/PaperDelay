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
    apvts.addParameterListener("TimeSync", this);
    apvts.addParameterListener("TimeChoice", this);
}

PaperDelayAudioProcessor::~PaperDelayAudioProcessor()
{
    apvts.removeParameterListener("Time", this);
    apvts.removeParameterListener("TimeSync", this);
    apvts.removeParameterListener("TimeChoice", this);
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
    
    dryWetMixer.prepare(spec);
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
    
    BPM = getPlayHead()->getPosition()->getBpm();
    
    juce::AudioBuffer<float> dryBuffer;
    dryBuffer.makeCopyOf(buffer);
    juce::dsp::AudioBlock<float> dryBlock(dryBuffer);
    
    dryWetMixer.pushDrySamples(dryBlock);
    
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
    
    juce::dsp::AudioBlock<float> block(buffer);
    
    dryWetMixer.setWetMixProportion(apvts.getRawParameterValue("WetAmount")->load());
    
    dryWetMixer.mixWetSamples(block);
}

//==============================================================================
bool PaperDelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PaperDelayAudioProcessor::createEditor()
{
    return new PaperDelayAudioProcessorEditor (*this);
//    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void PaperDelayAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    juce::MemoryOutputStream mos(destData, true);
    apvts.state.writeToStream(mos);
}

void PaperDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
    
    if (tree.isValid())
    {
        apvts.replaceState(tree);
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PaperDelayAudioProcessor();
}

void PaperDelayAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    if (apvts.getRawParameterValue("TimeChoice")->load() == 0)
    {
        delay.setDelay(calculateTimeToSamples(apvts.getRawParameterValue("Time")->load()));
    }
    else
    {
        beat = static_cast<Beat>(apvts.getRawParameterValue("TimeSync")->load());
        delay.setDelay(calculateBPMToSamples(beat));
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout PaperDelayAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Time", 1), "Time", juce::NormalisableRange<float>(1.f, 5000.f, 1.f, 1.f), 500.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("Feedback", 2), "Feedback", juce::NormalisableRange<float>(0.f, 1.f, 0.01f, 1.f), 0.2f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("WetAmount", 3), "Wet Amount", juce::NormalisableRange<float>(0.f, 1.f, 0.01f, 1.f), 1.f));
    
    juce::StringArray timeSyncChoices;
    
    timeSyncChoices.add("1/1");
    timeSyncChoices.add("1/2");
    timeSyncChoices.add("1/2T");
    timeSyncChoices.add("1/4");
    timeSyncChoices.add("1/4T");
    timeSyncChoices.add("1/8");
    timeSyncChoices.add("1/8T");
    timeSyncChoices.add("1/16");
    timeSyncChoices.add("1/16T");
    timeSyncChoices.add("1/32");
    timeSyncChoices.add("1/64");
    
    layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("TimeSync", 4), "Time Sync", timeSyncChoices, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("TimeChoice", 5), "Time Choice", juce::StringArray("ms", "sync"), 0));
    
    return layout;
}

float PaperDelayAudioProcessor::calculateTimeToSamples(float delayInMilliseconds)
{
    const auto sampleRate = getSampleRate();
    
    return static_cast<float>(delayInMilliseconds * (sampleRate / 1000));
}

float PaperDelayAudioProcessor::calculateBPMToSamples(Beat &beat)
{
    float quarterNoteTime = 60000.f / *BPM;
    
    switch (beat)
    {
        case fullNote:
        {
            return calculateTimeToSamples(quarterNoteTime * 4);
            break;
        }
            
        case halfNote:
        {
            return calculateTimeToSamples(quarterNoteTime * 2);
            break;
        }
            
        case halfNoteT:
        {
            return calculateTimeToSamples((quarterNoteTime / 3) * 4);
            break;
        }
            
        case quarterNote:
        {
            return calculateTimeToSamples(quarterNoteTime);
            break;
        }
            
        case quarterNoteT:
        {
            return calculateTimeToSamples((quarterNoteTime / 3) * 2);
            break;
        }
            
        case eigthNote:
        {
            return calculateTimeToSamples(quarterNoteTime / 2);
            break;
        }
            
        case eigthNoteT:
        {
            return calculateTimeToSamples(quarterNoteTime / 3);
            break;
        }
            
        case sixteenthNote:
        {
            return calculateTimeToSamples(quarterNoteTime / 4);
            break;
        }
            
        case sixteenthNoteT:
        {
            return calculateTimeToSamples(quarterNoteTime / 6);
            break;
        }
            
        case thirtyTwoNote:
        {
            return calculateTimeToSamples(quarterNoteTime / 8);
            break;
        }
            
        case sixtyFourNote:
        {
            return calculateTimeToSamples(quarterNoteTime / 16);
            break;
        }
    }
}
