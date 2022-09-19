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
OscComponent::OscComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorId, juce::String fmFreqId, juce::String fmDepthId)
{
    // The 3 choices for different waveforms
    juce::StringArray choices{ "Sine", "Saw", "Square" };

    // Add the choices string array to the oscWaveSelector
    oscWaveSelector.addItemList(choices, 1);

    // Finally add and make visible the combo box
    addAndMakeVisible(oscWaveSelector);

    // Allows you to link up the choice that the user makes with the actual visible representation that they see.
    oscWaveSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, waveSelectorId, oscWaveSelector);

    setSliderWithLabel(fmFreqSlider, fmFreqLabel, apvts, fmFreqId, fmFreqAttachment);
    setSliderWithLabel(fmDepthSlider, fmDepthLabel, apvts, fmDepthId, fmDepthAttachment);
}

OscComponent::~OscComponent()
{
}

void OscComponent::paint (juce::Graphics& g)
{
    // Paint the background of the box white.
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);

    // Useful for seeing the size of a component
    g.drawRect(getLocalBounds());
}

void OscComponent::resized()
{
    // Variables to hold the information that is shared across the sliders and labels
    const auto sliderPosY = 80;
    const auto sliderWidth = 100;
    const auto sliderHeight = 90;
    const auto labelYOffset = 20;
    const auto labelHeight = 20;

    // Set its bounds to be 90 by 20
    oscWaveSelector.setBounds(0, 0, 90, 20);

    // Set the bounds of the FM Frequency knob
    fmFreqSlider.setBounds(0, sliderPosY, sliderWidth, sliderHeight);
    fmFreqLabel.setBounds(fmFreqSlider.getX(), fmFreqSlider.getY() - labelYOffset, fmFreqSlider.getWidth(), labelHeight);

    // Set the bounds of the FM Depth knob
    fmDepthSlider.setBounds(fmFreqSlider.getRight(), sliderPosY, sliderWidth, sliderHeight);
    fmDepthLabel.setBounds(fmDepthSlider.getX(), fmDepthSlider.getY() - labelYOffset, fmDepthSlider.getWidth(), labelHeight);

}

void OscComponent::setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramId, std::unique_ptr<Attachment>& attachment)
{
    // Sets the slider style and makes it visible
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, this, 50, 25);
    addAndMakeVisible(slider);

    // Sets the attachment to the apvts
    attachment = std::make_unique<Attachment>(apvts, paramId, slider);

    // Sets the Label style and makes it visible
    label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    label.setFont(15.0f);
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);
}
