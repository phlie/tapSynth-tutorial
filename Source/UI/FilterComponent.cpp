/*
  ==============================================================================

    FilterComponent.cpp
    Created: 24 Sep 2022 2:57:14am
    Author:  phlie

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterComponent.h"

//==============================================================================
FilterComponent::FilterComponent(juce::AudioProcessorValueTreeState& apvts, juce::String filterTypeSelectorId,
    juce::String filterCutoffId, juce::String filterResId)
{
    // Initialize the Type with the three choices of filter.
    juce::StringArray choices{ "Low-pass", "Band-pass", "High-pass" };
    // Add the items to the list with the default being Low-pass
    filterTypeSelector.addItemList(choices, 1);

    // Sets its background color to be black.
    filterTypeSelector.setColour(juce::ComboBox::ColourIds::backgroundColourId, juce::Colours::black);

    // Add the Filter Type Selector
    addAndMakeVisible(filterTypeSelector);

    // Create an attachment for the ComboBox with the correct ID and the correct ComboBox object
    filterTypeSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, filterTypeSelectorId, filterTypeSelector);

    // Set the color of the font and font properties and add and make visible.
    filterSelectorLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    filterSelectorLabel.setFont(15.0f);
    filterSelectorLabel.setJustificationType(juce::Justification::left);
    addAndMakeVisible(filterSelectorLabel);

    // Finally, for the two custom knobs, use the custom knob class.
    setSliderWithLabel(filterCutoffSlider, filterCutoffLabel, apvts, filterCutoffId, filterCutoffAttachment);
    setSliderWithLabel(filterResonanceSlider, filterResonanceLabel, apvts, filterResId, filterResonanceAttachment);
}

FilterComponent::~FilterComponent()
{
}

void FilterComponent::paint (juce::Graphics& g)
{
    // Similar to the AdsrComponent class
    auto bounds = getLocalBounds().reduced(5);
    auto labelSpace = bounds.removeFromTop(25.0f);

    //g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawText("Filter", labelSpace.withX(5), juce::Justification::left);
    g.drawRoundedRectangle(bounds.toFloat(), 5.0f, 5.0f);
}

void FilterComponent::resized()
{
    // See AdsrComponent classes explanation.
    const auto startY = 55;
    const auto sliderWidth = 100;
    const auto sliderHeight = 90;
    const auto labelYOffset = 20;
    const auto labelHeight = 20;

    filterTypeSelector.setBounds(10, startY + 5, 90, 30);
    filterSelectorLabel.setBounds(10, startY - labelYOffset, 90, labelHeight);

    filterCutoffSlider.setBounds(filterTypeSelector.getRight(), startY, sliderWidth, sliderHeight);
    filterCutoffLabel.setBounds(filterCutoffSlider.getX(), filterCutoffSlider.getY() - labelYOffset, filterCutoffSlider.getWidth(), labelHeight);

    filterResonanceSlider.setBounds(filterCutoffSlider.getRight(), startY, sliderWidth, sliderHeight);
    // By setting the label to be the same width as the slider, with center justification the text while appear exactly in the middle relative to its slider.
    filterResonanceLabel.setBounds(filterResonanceSlider.getX(), filterResonanceSlider.getY() - labelYOffset,
        filterResonanceSlider.getWidth(), labelHeight);
}

// The custom Slider settings function
void FilterComponent::setSliderWithLabel(juce::Slider& slider, juce::Label& label,
    juce::AudioProcessorValueTreeState& apvts, juce::String paramId, std::unique_ptr<Attachment>& attachment)
{
    // Set the Sliders and its own textbox style, as well as set the colors and initialize.
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    slider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::aqua);
    slider.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::black);
    slider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::silver);
    addAndMakeVisible(slider);

    // Create an attachment for the Slider with its parameter ID and the slider object itself.
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramId, slider);

    // Setup the label and make it visible.
    label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    label.setFont(15.0f);
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);
}