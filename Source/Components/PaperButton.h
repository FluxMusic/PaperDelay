/*
  ==============================================================================

    PaperButton.h
    Created: 6 May 2024 11:02:56pm
    Author:  Alex Huber

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../LookAndFeel/PaperLookAndFeel.h"

class PaperButton : public juce::Button
{
public:
    PaperButton(juce::String displayText);
    
    ~PaperButton();
    
    void paintButton (juce::Graphics& g, bool, bool) override;
    
private:
    PaperLookAndFeel lnf;
    
    juce::String label;
};
