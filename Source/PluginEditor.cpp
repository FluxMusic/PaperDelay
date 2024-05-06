/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PaperDelayAudioProcessorEditor::PaperDelayAudioProcessorEditor (PaperDelayAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
feedbackSlider(*audioProcessor.getAPVTS().getParameter("Feedback")),
dryWetSlider(*audioProcessor.getAPVTS().getParameter("WetAmount")),
timeMSSlider(*audioProcessor.getAPVTS().getParameter("Time")),
feedbackSliderAttachment(audioProcessor.getAPVTS(), "Feedback", feedbackSlider),
dryWetSliderAttachment(audioProcessor.getAPVTS(), "WetAmount", dryWetSlider),
timeMSSliderAttachment(audioProcessor.getAPVTS(), "Time", timeMSSlider)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    addAndMakeVisible(timeMSSlider);
    addAndMakeVisible(msButton);
    addAndMakeVisible(syncButton);
    addAndMakeVisible(feedbackSlider);
    addAndMakeVisible(dryWetSlider);
    
    getConstrainer()->setFixedAspectRatio(0.93);
    setResizable(true, true);
    setResizeLimits(400, 430, 800, 860);
    setSize (400, 430);
}

PaperDelayAudioProcessorEditor::~PaperDelayAudioProcessorEditor()
{
}

//==============================================================================
void PaperDelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::floralwhite);
    const auto fullBounds = getLocalBounds();
    
    auto bounds = fullBounds;
    
    auto timeBounds = bounds.removeFromTop(bounds.getHeight() / 2);
    timeBounds.removeFromLeft(fullBounds.getWidth() / 8);
    timeBounds.removeFromRight(fullBounds.getWidth() / 8);
    timeBounds.removeFromTop(timeBounds.getHeight() / 5);
    auto msButtonBounds = timeBounds.removeFromBottom(timeBounds.getHeight() / 3);
    auto syncButtonBounds = msButtonBounds.removeFromRight(msButtonBounds.getWidth() / 2);
    
    g.setColour(juce::Colours::red);
    g.fillRect(timeBounds);
    
    g.setColour (juce::Colours::darkgrey);
    g.setFont (20.0f);
    g.drawFittedText ("Paper Delay", getLocalBounds().removeFromTop(30), juce::Justification::centred, 1);
    
    feedbackSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::dimgrey);
    
}

void PaperDelayAudioProcessorEditor::resized()
{
    const auto fullBounds = getLocalBounds();
    
    auto bounds = fullBounds;
    
    auto timeBounds = bounds.removeFromTop(bounds.getHeight() / 2);
    timeBounds.removeFromLeft(fullBounds.getWidth() / 8);
    timeBounds.removeFromRight(fullBounds.getWidth() / 8);
    timeBounds.removeFromTop(timeBounds.getHeight() / 5);
    auto msButtonBounds = timeBounds.removeFromBottom(timeBounds.getHeight() / 3);
    auto syncButtonBounds = msButtonBounds.removeFromRight(msButtonBounds.getWidth() / 2);
    
//    timeLabel.setBounds(timeBounds);
//    timeLabel.setJustificationType(juce::Justification::centred);
    
    timeMSSlider.setBounds(timeBounds);
    
    msButton.setBounds(msButtonBounds);
    syncButton.setBounds(syncButtonBounds);
    
    auto feedbackSliderBounds = bounds.removeFromTop((bounds.getHeight() / 3) * 2);
    
    bounds.removeFromLeft(fullBounds.getWidth() / 7);
    bounds.removeFromRight(fullBounds.getWidth() / 7);
    
    feedbackSlider.setBounds(feedbackSliderBounds);
    dryWetSlider.setBounds(bounds);
}
