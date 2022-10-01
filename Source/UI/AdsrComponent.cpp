/*
  ==============================================================================

    AdsrComponent.cpp
    Created: 6 Sep 2022 9:14:20am
    Author:  phlie

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AdsrComponent.h"

//==============================================================================
AdsrComponent::AdsrComponent(juce::String name, juce::AudioProcessorValueTreeState& apvts, juce::String attackId, juce::String decayId, juce::String sustainId, juce::String releaseId)
{
    // Gives this component a name.
    componentName = name;
    

    // Attach each audioProcessor element to its own front-end knob or combobox
    attackAttachment = std::make_unique<SliderAttachment>(apvts, attackId, attackSlider);
    decayAttachment = std::make_unique<SliderAttachment>(apvts, decayId, decaySlider);
    sustainAttachment = std::make_unique<SliderAttachment>(apvts, sustainId, sustainSlider);
    releaseAttachment = std::make_unique<SliderAttachment>(apvts, releaseId, releaseSlider);

    // Set the Slider's parameters using our custom class we created for easier ease of setting up sliders.
    setSliderParams(attackSlider);
    setSliderParams(decaySlider);
    setSliderParams(sustainSlider);
    setSliderParams(releaseSlider);

}

AdsrComponent::~AdsrComponent()
{
}

void AdsrComponent::paint (juce::Graphics& g)
{
    // Transparent if this is commented out to show the background.
    //g.fillAll(juce::Colours::black);

    // Sets the drawing color as white.
    g.setColour(juce::Colours::white);

    // A Rectangle that is 5 pixels smaller on each side then the total size of the component.
    auto bounds = getLocalBounds().reduced(5);

    // Reduces the top of the rect by 25.0f and returns a space for the label to go which is the removed part.
    auto labelSpace = bounds.removeFromTop(25.0f);

    // Sets the font to size 20
    g.setFont(20.0f);

    // Draws the component name on the left within the label space but 5 pixels to the right of where labelSpace starts.
    g.drawText(componentName, labelSpace.withX(5), juce::Justification::left);

    // Draws a rectangle border that is rounded to be 5.0f in radius and also has a line thickness of 5.0f as well.
    // Its bounds is just the remainder of space after the top has been removed of bounds.
    g.drawRoundedRectangle(bounds.toFloat(), 5.0f, 5.0f);
}

// Called from within PluginEditor with it having its own bounds.
void AdsrComponent::resized()
{
    /*
    // Setup variables for automatic positioning of the ADSR sliders.
    const auto bounds = getLocalBounds().reduced(10);
    const auto padding = 10;
    const auto sliderWidth = bounds.getWidth() / 4 - padding;
    const auto sliderHeight = bounds.getHeight() / 2;
    const auto sliderStartX = 0;

    //const auto sliderStartY = bounds.getHeight() / 2 - (sliderHeight / 2);
    const auto sliderStartY = 0;

    // Although they are set as exact pixel values they alter their position easily
    attackSlider.setBounds(sliderStartX, sliderStartY, sliderWidth, sliderHeight);
    decaySlider.setBounds(attackSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    sustainSlider.setBounds(decaySlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    releaseSlider.setBounds(sustainSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    */

    // The bounds reduced by 10 for margin on each side.
    const auto bounds = getLocalBounds().reduced(10);

    // The padding
    const auto padding = 10;

    // Each slider takes up 1/4 of the width
    const auto sliderWidth = bounds.getWidth() / 4 - padding;

    // Each slider takes up the full height - 45.
    const auto sliderHeight = bounds.getHeight() - 45;

    // It starts 5 pixels after the padding ends
    const auto sliderStartX = padding + 5;

    // Starts 55 pixels from the top of the bounds
    const auto sliderStartY = 55;
    const auto labelYOffset = 20;
    const auto labelHeight = 20;
    const auto labelStart = sliderStartY - labelYOffset;

    // The slider starts at sliderStartX and sliderStartY, then has a set width, and finally a set height.
    attackSlider.setBounds(sliderStartX, sliderStartY, sliderWidth, sliderHeight);
    //attackLabel.setBounds(attackSlider.getX(), labelStart, sliderWidth, labelHeight);

    // Same as above except it starts on the right hand side of the slider above.
    decaySlider.setBounds(attackSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    //decayLabel.setBounds(decaySlider.getX(), labelStart, sliderWidth, labelHeight);

    sustainSlider.setBounds(decaySlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    //sustainLabel.setBounds(sustainSlider.getX(), labelStart, sliderWidth, labelHeight);

    releaseSlider.setBounds(sustainSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    //releaseLabel.setBounds(releaseSlider.getX(), labelStart, sliderWidth, labelHeight);
}

// Used to clean up the code to setup the slider params.
void AdsrComponent::setSliderParams(juce::Slider& slider)
{
    // First set the slider style
    slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);

    // Set the TextBox below and make it read only.
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);

    // The colors for the various parameters to make it look pretty.
    slider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::aqua);
    slider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::black);
    slider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::silver);

    // Finally, add and make visible the slider.
    addAndMakeVisible(slider);
}