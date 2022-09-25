/*
  ==============================================================================

    FilterData.h
    Created: 24 Sep 2022 2:17:46am
    Author:  phlie

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class FilterData
{
public:
    // Used to pass the sample rate and buffer size to the DSP algorithm.
    void prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels);
    void process(juce::AudioBuffer<float>& buffer);
    void updateParameters(const int filterType, const float frequency, const float resonance, const float modulator);
    void reset();

private:
    juce::dsp::StateVariableTPTFilter<float> filter;
    bool isPrepared{ false };
};
