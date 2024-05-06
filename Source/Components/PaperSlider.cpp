/*
  ==============================================================================

    PaperSlider.cpp
    Created: 6 May 2024 12:44:57pm
    Author:  Alex Huber

  ==============================================================================
*/

#include "PaperSlider.h"

PaperSlider::PaperSlider(juce::RangedAudioParameter& rap)
: juce::Slider(juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextEntryBoxPosition::NoTextBox),
param(&rap)
{
    setLookAndFeel(&lnf);
}

PaperSlider::~PaperSlider()
{
    setLookAndFeel(nullptr);
    
//    param = nullptr;
}

void PaperSlider::paint(juce::Graphics& g)
{
    const auto bounds = getLocalBounds();
    
    const auto minSliderPos = static_cast<double>(bounds.getX() + bounds.getWidth() / 23);
    const auto maxSliderPos = static_cast<double>(bounds.getWidth() - bounds.getWidth() / 23);
    
    const auto range = getRange();
    
    getLookAndFeel().drawLinearSlider(g,
                                      bounds.getX(),
                                      bounds.getY(),
                                      bounds.getWidth(),
                                      bounds.getHeight(),
                                      juce::jmap(getValue(), range.getStart(), range.getEnd(), minSliderPos, maxSliderPos),
                                      minSliderPos,
                                      maxSliderPos,
                                      juce::Slider::SliderStyle::LinearHorizontal,
                                      *this);
}
