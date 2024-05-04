/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

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
    
    juce::Slider feedbackSlider {juce::Slider::SliderStyle::RotaryVerticalDrag, juce::Slider::TextEntryBoxPosition::TextBoxAbove};
    juce::Slider dryWetSlider {juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextEntryBoxPosition::NoTextBox};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PaperDelayAudioProcessorEditor)
};
