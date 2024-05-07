/*
  ==============================================================================

    PaperLookAndFeel.cpp
    Created: 5 May 2024 5:42:04pm
    Author:  Alex Huber

  ==============================================================================
*/

#include "PaperLookAndFeel.h"
#include "../Components/PaperKnob.h"
#include "../Components/PaperFieldSlider.h"

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
    
    if (dynamic_cast<PaperKnob*>(&slider))
    {
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
    else if (dynamic_cast<PaperFieldSlider*>(&slider))
    {
        if (const auto windowSVG = juce::XmlDocument::parse(BinaryData::SyncButtons_svg))
        {
            const auto drawable = juce::Drawable::createFromSVG(*windowSVG);
            drawable->setTransformToFit(originalBounds.toFloat(), juce::RectanglePlacement::centred);
            drawable->draw(g, 1.f);
        }
        
        g.setColour(juce::Colours::floralwhite);
        g.fillRect(originalBounds.reduced(originalBounds.getWidth() / 50));
    }
}

void PaperLookAndFeel::drawLinearSlider(juce::Graphics& g,
                                        int x,
                                        int y,
                                        int width,
                                        int height,
                                        float sliderPos,
                                        float minSliderPos,
                                        float maxSliderPos,
                                        juce::Slider::SliderStyle,
                                        juce::Slider& slider)
{
    const auto bounds = juce::Rectangle<int>(x, y, width, height);
    
    const auto size = juce::jmin(width, height) / 3;
    
    auto knobBounds = juce::Rectangle<int>(size, size);
    knobBounds.setX(sliderPos - size / 2);
    knobBounds.setY((bounds.getCentreY() - size / 2) - size / 20);
    
    if (const auto slider = juce::XmlDocument::parse(BinaryData::DryWetSlider_svg))
    {
        const auto drawable = juce::Drawable::createFromSVG(*slider);
        drawable->setTransformToFit(bounds.toFloat(), juce::RectanglePlacement::centred);
        drawable->draw(g, 1.f);
    }
    
    if (const auto sliderKnob = juce::XmlDocument::parse(BinaryData::DryWetSliderKnob_svg))
    {
        const auto drawable = juce::Drawable::createFromSVG(*sliderKnob);
        g.setColour(juce::Colours::floralwhite);
        g.fillEllipse(knobBounds.reduced(size / 20).toFloat());
        drawable->setTransformToFit(knobBounds.toFloat(), juce::RectanglePlacement::centred);
        drawable->draw(g, 1.f);
    }
}

void PaperLookAndFeel::drawButtonBackground(juce::Graphics& g,
                                            juce::Button& button,
                                            const juce::Colour& backgroundColour,
                                            bool shouldDrawButtonAsHighlighted,
                                            bool shouldDrawButtonAsDown)
{
    const auto bounds = button.getLocalBounds();
    
    if (button.getToggleState())
    {
        g.setColour(backgroundColour.darker(0.3));
    }
    else
    {
        g.setColour(backgroundColour);
    }
    g.fillRect(bounds.reduced(bounds.getWidth() / 50));
    
    if (const auto buttonSVG = juce::XmlDocument::parse(BinaryData::SyncButtons_svg))
    {
        const auto drawable = juce::Drawable::createFromSVG(*buttonSVG);
        drawable->setTransformToFit(bounds.toFloat(), juce::RectanglePlacement::centred);
        drawable->draw(g, 1.f);
    }
}
