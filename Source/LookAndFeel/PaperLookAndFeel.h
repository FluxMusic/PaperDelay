/*
  ==============================================================================

    PaperLookAndFeel.h
    Created: 5 May 2024 5:42:04pm
    Author:  Alex Huber

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class PaperLookAndFeel : juce::LookAndFeel_V4
{
    void drawRotarySlider (juce::Graphics&, int x, int y, int width, int height,
                           float sliderPosProportional, float rotaryStartAngle,
                           float rotaryEndAngle, juce::Slider&) override;
};
