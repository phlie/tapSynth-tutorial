/*
  ==============================================================================

    AdsrComponent.h
    Created: 6 Sep 2022 9:14:20am
    Author:  phlie

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class AdsrComponent  : public juce::Component
{
public:
    AdsrComponent(juce::AudioProcessorValueTreeState& apvts);
    ~AdsrComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // The method to set the slider's params.
    void setSliderParams(juce::Slider& slider);

    // The sliders
    juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;

    // An alias that shortens the following commands quite well
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    // Make a audioProcessorValueTreeState::Sliderattachment called attackAttachment etc.
    std::unique_ptr<SliderAttachment> attackAttachment;
    std::unique_ptr<SliderAttachment> decayAttachment;
    std::unique_ptr<SliderAttachment> sustainAttachment;
    std::unique_ptr<SliderAttachment> releaseAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AdsrComponent)
};
