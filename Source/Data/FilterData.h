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

    // Given the input of an Audio Buffer it outputs the effects of the filter on the Audio Buffer
    void process(juce::AudioBuffer<float>& buffer);

    // Used to update the basic parameters of the filter as well as modulation for the frequency.
    void updateParameters(const int filterType, const float frequency, const float resonance, const float modulator);
    
    // Resets the filter
    void reset();

private:
    // The type of the filter, both float and TPT.
    juce::dsp::StateVariableTPTFilter<float> filter;

    // Turns true once the prepareToPlay method has been called on the filter.
    bool isPrepared{ false };
};
