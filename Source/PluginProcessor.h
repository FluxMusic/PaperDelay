/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class PaperDelayAudioProcessor  : public juce::AudioProcessor, juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    PaperDelayAudioProcessor();
    ~PaperDelayAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    juce::AudioProcessorValueTreeState& getAPVTS() { return apvts; }

private:
    juce::AudioProcessorValueTreeState apvts;
    
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    juce::Optional<float> BPM = 120;
    
    enum Beat
    {
        fullNote,
        halfNote,
        halfNoteT,
        quarterNote,
        quarterNoteT,
        eigthNote,
        eigthNoteT,
        sixteenthNote,
        sixteenthNoteT,
        thirtyTwoNote,
        sixtyFourNote
    } beat;
    
    float calculateTimeToSamples(float delayInMilliseconds);
    
    float calculateBPMToSamples(Beat& beat);
    
    void parameterChanged(const juce::String& parameterID, float newValue) override;
    
    juce::dsp::DelayLine<float> delay;
    
    juce::dsp::DryWetMixer<float> dryWetMixer;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PaperDelayAudioProcessor)
};
