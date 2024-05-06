/*
  ==============================================================================

    PaperFieldSlider.h
    Created: 6 May 2024 2:28:05pm
    Author:  Alex Huber

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../LookAndFeel/PaperLookAndFeel.h"

class PaperFieldSlider : public juce::Slider
{
public:
    PaperFieldSlider(juce::RangedAudioParameter& rap);
    
    ~PaperFieldSlider();
    
    void paint (juce::Graphics&) override;
    
private:
    PaperLookAndFeel lnf;
    
    juce::RangedAudioParameter* param;
};
