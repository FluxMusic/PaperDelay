/*
  ==============================================================================

    PaperLookAndFeel.cpp
    Created: 5 May 2024 5:42:04pm
    Author:  Alex Huber

  ==============================================================================
*/

#include "PaperLookAndFeel.h"

void PaperLookAndFeel::drawRotarySlider(juce::Graphics& g, 
                                        int x,
                                        int y,
                                        int width,
                                        int height,
                                        float sliderPosProportional,
                                        float rotaryStartAngle,
                                        float rotaryEndAngle,
                                        juce::Slider& slider)
{
    const auto originalBounds = juce::Rectangle<int>(x, y, width, height);
    
    auto size = juce::jmin(originalBounds.getWidth(), originalBounds.getHeight());
    
    auto bounds = juce::Rectangle<int>(size, size);
    
    bounds.setCentre(originalBounds.getCentre());
    
    if (const auto svg = juce::XmlDocument::parse(BinaryData::Knob_svg))
    {
        const auto drawable = juce::Drawable::createFromSVG(*svg);
        drawable->setTransformToFit(bounds.toFloat(), juce::RectanglePlacement::centred);
        
        const float angle = juce::jmap(sliderPosProportional, 0.f, 1.f, rotaryStartAngle, rotaryEndAngle);
        
        drawable->draw(g, 1.f, juce::AffineTransform::rotation(angle, bounds.getCentreX(), bounds.getCentreY()));
    }
}
