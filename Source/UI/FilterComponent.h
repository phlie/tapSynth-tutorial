/*
  ==============================================================================

    FilterComponent.h
    Created: 24 Sep 2022 2:57:14am
    Author:  phlie

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class FilterComponent  : public juce::Component
{
public:
    // Its constructor takes a reference to ValueTreeState, and what its initial parameters are.
    FilterComponent(juce::AudioProcessorValueTreeState& apvts, juce::String filterTypeSelectorId,
        juce::String filterCutoffId, juce::String filterResId);
    ~FilterComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;


private:
    // The ComboBox to select the Filter Type.
    juce::ComboBox filterTypeSelector{ "Filter Type"};

    // A ComboBox attachmetn
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterTypeSelectorAttachment;

    // Shorten the Slider Attachment string
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    // Two sliders, one for the Cutoff Frequency, and one for the Resonance Frequency
    juce::Slider filterCutoffSlider;
    juce::Slider filterResonanceSlider;

    // Two slider attachments for the two sliders above.
    std::unique_ptr<Attachment> filterCutoffAttachment;
    std::unique_ptr<Attachment> filterResonanceAttachment;

    // The labels for all three choices.
    juce::Label filterSelectorLabel{ "Filter Type", "Filter Type" };
    juce::Label filterCutoffLabel{ "Filter Freq", "Filter Freq" };
    juce::Label filterResonanceLabel{ "Filter Res", "Filter Res" };

    // Finally, a custom class to make it easier to assign slider parameters.
    void setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramId, std::unique_ptr<Attachment>& attachment);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterComponent)
};
