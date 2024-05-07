/*
  ==============================================================================

    PaperKnob.cpp
    Created: 5 May 2024 5:41:45pm
    Author:  Alex Huber

  ==============================================================================
*/

#include "PaperKnob.h"

PaperKnob::PaperKnob(juce::RangedAudioParameter& rap)
: juce::Slider(juce::Slider::SliderStyle::RotaryVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox),
param(&rap)
{
    setLookAndFeel(&lnf);
}

PaperKnob::~PaperKnob()
{
    setLookAndFeel(nullptr);
    
//    param = nullptr;
}

void PaperKnob::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    
    const auto textBounds = bounds.removeFromTop(bounds.getHeight() / 4);
    
    const auto startAngle = juce::degreesToRadians(180.f + 30.f);
    const auto endAngle = juce::degreesToRadians(180.f - 30.f) + juce::MathConstants<float>::twoPi;
    
    const auto range = getRange();
    
    getLookAndFeel().drawRotarySlider(g,
                                      bounds.getX(),
                                      bounds.getY(),
                                      bounds.getWidth(),
                                      bounds.getHeight(),
                                      juce::jmap(getValue(), range.getStart(), range.getEnd(), 0.0, 1.0),
                                      startAngle,
                                      endAngle,
                                      *this);
    
    g.setColour(juce::Colours::black);
    g.setFont(textBounds.getHeight() / 1.3);
    g.drawFittedText("Feedback", textBounds, juce::Justification::centred, 1);
}
