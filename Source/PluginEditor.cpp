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
timeMSSlider(*audioProcessor.getAPVTS().getParameter("Time")),
timeSyncSlider(*audioProcessor.getAPVTS().getParameter("TimeSync")),
feedbackSlider(*audioProcessor.getAPVTS().getParameter("Feedback")),
dryWetSlider(*audioProcessor.getAPVTS().getParameter("WetAmount")),
feedbackSliderAttachment(audioProcessor.getAPVTS(), "Feedback", feedbackSlider),
dryWetSliderAttachment(audioProcessor.getAPVTS(), "WetAmount", dryWetSlider),
timeMSSliderAttachment(audioProcessor.getAPVTS(), "Time", timeMSSlider),
timeSyncSliderAttachment(audioProcessor.getAPVTS(), "TimeSync", timeSyncSlider)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    addAndMakeVisible(timeSyncSlider);
    addAndMakeVisible(timeMSSlider);
    addAndMakeVisible(msButton);
    addAndMakeVisible(syncButton);
    addAndMakeVisible(feedbackSlider);
    addAndMakeVisible(dryWetSlider);
    
    msButton.setRadioGroupId(1);
    syncButton.setRadioGroupId(1);
    
    auto syncValue = static_cast<int>(audioProcessor.getAPVTS().getRawParameterValue("TimeChoice")->load());
    
    switch (syncValue)
    {
        case 0:
            msButton.setToggleState(true, juce::NotificationType::dontSendNotification);
            timeSyncSlider.toBehind(&timeMSSlider);
            break;
            
        case 1:
            syncButton.setToggleState(true, juce::NotificationType::dontSendNotification);
            timeMSSlider.toBehind(&timeSyncSlider);
            
        default:
            break;
    }

    msButton.onClick = [this]()
    {
        if (!msButton.getToggleState())
        {
            msButton.setToggleState(true, juce::NotificationType::dontSendNotification);
            audioProcessor.getAPVTS().getParameter("TimeChoice")->setValueNotifyingHost(0);
            timeSyncSlider.toBehind(&timeMSSlider);
        }
    };
    
    syncButton.onClick = [this]()
    {
        if (!syncButton.getToggleState())
        {
            syncButton.setToggleState(true, juce::NotificationType::dontSendNotification);
            audioProcessor.getAPVTS().getParameter("TimeChoice")->setValueNotifyingHost(1);
            timeMSSlider.toBehind(&timeSyncSlider);
        }
    };
    
    getConstrainer()->setFixedAspectRatio(0.86);
    setResizable(true, true);
    setResizeLimits(370, 430, 740, 860);
    setSize (370, 430);
}

PaperDelayAudioProcessorEditor::~PaperDelayAudioProcessorEditor()
{
}

//==============================================================================
void PaperDelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::floralwhite);
    
    auto bounds = getLocalBounds();
    
    auto outlineBounds = getLocalBounds().reduced(getLocalBounds().getWidth() / 40);
    g.setColour(juce::Colours::black);
    g.drawRoundedRectangle(outlineBounds.toFloat(), 15.f, bounds.getWidth() / 100);
    
    bounds.removeFromTop(bounds.getWidth() / 60);
    auto titleBounds = bounds.removeFromTop(getHeight() / 10);
    
    g.setColour (juce::Colours::black);
    g.setFont (titleBounds.getHeight() / 2);
    g.drawFittedText ("Paper Delay", titleBounds, juce::Justification::centred, 1);
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
    
    timeMSSlider.setBounds(timeBounds);
    timeSyncSlider.setBounds(timeBounds);
    
    msButton.setBounds(msButtonBounds);
    syncButton.setBounds(syncButtonBounds);
    
    auto feedbackSliderBounds = bounds.removeFromTop((bounds.getHeight() / 3) * 2);
    
    bounds.removeFromLeft(fullBounds.getWidth() / 7);
    bounds.removeFromRight(fullBounds.getWidth() / 7);
    
    feedbackSlider.setBounds(feedbackSliderBounds);
    dryWetSlider.setBounds(bounds);
}
