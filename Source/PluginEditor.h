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
#include "Components/PaperFieldSlider.h"
#include "Components/PaperButton.h"

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
    
    PaperFieldSlider timeMSSlider, timeSyncSlider;
    
    PaperButton msButton {"MS"};
    PaperButton syncButton {"SYNC"};
    
    PaperKnob feedbackSlider;
    PaperSlider dryWetSlider;
    
    juce::AudioProcessorValueTreeState::SliderAttachment feedbackSliderAttachment, dryWetSliderAttachment, timeMSSliderAttachment, timeSyncSliderAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PaperDelayAudioProcessorEditor)
};
