/*
  ==============================================================================

    PaperButton.cpp
    Created: 6 May 2024 11:02:56pm
    Author:  Alex Huber

  ==============================================================================
*/

#include "PaperButton.h"

PaperButton::PaperButton(juce::String displayText)
: juce::Button(displayText),
label(displayText)
{
    setLookAndFeel(&lnf);
}

PaperButton::~PaperButton()
{
    setLookAndFeel(nullptr);
    
//    param = nullptr;
}

void PaperButton::paintButton(juce::Graphics& g, bool, bool)
{
    getLookAndFeel().drawButtonBackground(g, *this, juce::Colours::floralwhite, false, false);
    
    g.setColour(juce::Colours::black);
    g.drawText(label, getLocalBounds(), juce::Justification::centred);
}
