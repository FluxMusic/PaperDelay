/*
  ==============================================================================

    PaperSlider.cpp
    Created: 6 May 2024 12:44:57pm
    Author:  Alex Huber

  ==============================================================================
*/

#include "PaperSlider.h"

PaperSlider::PaperSlider(juce::RangedAudioParameter& rap)
: juce::Slider(juce::Slider::SliderStyle::LinearBar, juce::Slider::TextEntryBoxPosition::NoTextBox),
param(&rap)
{
    setLookAndFeel(&lnf);
    setSliderSnapsToMousePosition(false);
}

PaperSlider::~PaperSlider()
{
    setLookAndFeel(nullptr);
    
//    param = nullptr;
}

void PaperSlider::paint(juce::Graphics& g)
{
    const auto bounds = getLocalBounds();
    
    auto textBounds = bounds;
    textBounds.removeFromBottom(textBounds.getHeight() / 1.3);
    auto wetBounds = textBounds.removeFromRight(textBounds.getWidth() / 2);
    
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
                                      juce::Slider::SliderStyle::LinearBar,
                                      *this);
    
    g.setColour(juce::Colours::black);
    g.setFont(bounds.getHeight() / 3);
    g.drawFittedText("Dry", textBounds, juce::Justification::centred, 1);
    g.drawFittedText("Wet", wetBounds, juce::Justification::centred, 1);
}
