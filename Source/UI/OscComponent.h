/*
  ==============================================================================

    OscComponent.h
    Created: 18 Sep 2022 12:23:54pm
    Author:  phlie

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class OscComponent  : public juce::Component
{
public:
    // The default constructor needs a reference to AudioProcessorValueTreeState and whatever type of default wavetype it should use.
    OscComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorId, juce::String fmFreqId, juce::String fmDepthId);
    ~OscComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // Need both a ComboBox and an attachment to one.
    juce::ComboBox oscWaveSelector;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscWaveSelectorAttachment;

    // The sliders for fm frequency modulation
    juce::Slider fmFreqSlider;
    juce::Slider fmDepthSlider;

    // A Short hand version of the Slider Attachment type
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    // The attachments for FM Frequency Modulation
    std::unique_ptr<Attachment> fmFreqAttachment;
    std::unique_ptr<Attachment> fmDepthAttachment;

    // The labels for the FM Knobs
    juce::Label fmFreqLabel{ "FM Freq", "Fm Freq" };
    juce::Label fmDepthLabel{ "FM Depth", "FM Depth" };

    // A function used to ease up the typing of all the code necessary for a slider and label
    void setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramId, std::unique_ptr<Attachment>& attachment);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscComponent)
};
