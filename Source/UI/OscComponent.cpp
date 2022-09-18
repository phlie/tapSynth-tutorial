/*
  ==============================================================================

    OscComponent.cpp
    Created: 18 Sep 2022 12:23:54pm
    Author:  phlie

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OscComponent.h"

//==============================================================================
OscComponent::OscComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorId)
{
    // The 3 choices for different waveforms
    juce::StringArray choices{ "Sine", "Saw", "Square" };

    // Add the choices string array to the oscWaveSelector
    oscWaveSelector.addItemList(choices, 1);

    // Finally add and make visible the combo box
    addAndMakeVisible(oscWaveSelector);

    // Allows you to link up the choice that the user makes with the actual visible representation that they see.
    oscWaveSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, waveSelectorId, oscWaveSelector);

}

OscComponent::~OscComponent()
{
}

void OscComponent::paint (juce::Graphics& g)
{
    // Paint the background of the box white.
    g.fillAll(juce::Colours::white);
}

void OscComponent::resized()
{
    // Set its bounds to be 90 by 20
    oscWaveSelector.setBounds(0, 0, 90, 20);
}
