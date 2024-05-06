/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Components/PaperKnob.h"
#include "Components/PaperSlider.h"

//==============================================================================
/**
*/
class PaperDelayAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    PaperDelayAudioProcessorEditor (PaperDelayAudioProcessor&);
    ~PaperDelayAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PaperDelayAudioProcessor& audioProcessor;
    
    juce::Label timeLabel {"Label", "Test"};
    
    juce::TextButton msButton {"ms"};
    juce::TextButton syncButton {"sync"};
    
    PaperKnob feedbackSlider;
    PaperSlider dryWetSlider;
    
    juce::AudioProcessorValueTreeState::SliderAttachment feedbackSliderAttachment, dryWetSliderAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PaperDelayAudioProcessorEditor)
};
