/*
  ==============================================================================

    PaperFieldSlider.cpp
    Created: 6 May 2024 2:28:05pm
    Author:  Alex Huber

  ==============================================================================
*/

#include "PaperFieldSlider.h"

PaperFieldSlider::PaperFieldSlider(juce::RangedAudioParameter& rap)
: juce::Slider(juce::Slider::SliderStyle::RotaryVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox),
param(&rap)
{
    setLookAndFeel(&lnf);
}

PaperFieldSlider::~PaperFieldSlider()
{
    setLookAndFeel(nullptr);
    
//    param = nullptr;
}

void PaperFieldSlider::paint(juce::Graphics& g)
{
    const auto bounds = getLocalBounds();
    
    const auto startAngle = juce::degreesToRadians(180.f + 30.f);
    const auto endAngle = juce::degreesToRadians(180.f - 30.f) + juce::MathConstants<float>::twoPi;
    
    const auto range = getRange();
    
    juce::String valueAsText = "";
    
    getLookAndFeel().drawRotarySlider(g,
                                      bounds.getX(),    
                                      bounds.getY(),
                                      bounds.getWidth(),
                                      bounds.getHeight(),
                                      juce::jmap(getValue(), range.getStart(), range.getEnd(), 0.0, 1.0),
                                      startAngle,
                                      endAngle,
                                      *this);
    
    if (param->getParameterID() == "Time")
    {
        valueAsText = static_cast<juce::String>(getValue());
    }
    else
    {
        if (auto choice = dynamic_cast<juce::AudioParameterChoice*>(param))
        {
            valueAsText = choice->getCurrentChoiceName();
        }
    }
    
    g.setColour(juce::Colours::black);
    g.setFont(bounds.getHeight() / 2);
    g.drawText(valueAsText, bounds, juce::Justification::centred);
}
