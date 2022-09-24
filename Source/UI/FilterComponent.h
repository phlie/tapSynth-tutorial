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
    FilterComponent(juce::AudioProcessorValueTreeState& apvts, juce::String filterTypeSelectorId,
        juce::String filterCutoffId, juce::String filterResId);
    ~FilterComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;


private:
    juce::ComboBox filterTypeSelector{ "Filter Type"};
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterTypeSelectorAttachment;

    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    juce::Slider filterCutoffSlider;
    juce::Slider filterResonanceSlider;

    std::unique_ptr<Attachment> filterCutoffAttachment;
    std::unique_ptr<Attachment> filterResonanceAttachment;

    juce::Label filterSelectorLabel{ "Filter Type", "Filter Type" };
    juce::Label filterCutoffLabel{ "Filter Freq", "Filter Freq" };
    juce::Label filterResonanceLabel{ "Filter Res", "Filter Res" };

    void setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramId, std::unique_ptr<Attachment>& attachment);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterComponent)
};
