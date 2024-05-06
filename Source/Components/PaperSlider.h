/*
  ==============================================================================

    PaperSlider.h
    Created: 6 May 2024 12:44:57pm
    Author:  Alex Huber

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../LookAndFeel/PaperLookAndFeel.h"

class PaperSlider : public juce::Slider
{
public:
    PaperSlider(juce::RangedAudioParameter& rap);
    
    ~PaperSlider();
    
    void paint (juce::Graphics&) override;
    
private:
    PaperLookAndFeel lnf;
    
    juce::RangedAudioParameter* param;
};
