/*
  ==============================================================================

    FilterData.cpp
    Created: 24 Sep 2022 2:17:46am
    Author:  phlie

  ==============================================================================
*/

#include "FilterData.h"

void FilterData::prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels)
{
    // First reset the filter
    filter.reset();

    // Prepare a Process Spec with the samples per block, sample rate, and num channels.
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = numChannels;

    // Pass the Process Spec to the filter
    filter.prepare(spec);

    // Finally flip the boolean
    isPrepared = true;
}
void FilterData::process(juce::AudioBuffer<float>& buffer)
{
    // If this is not true, it will stop and throw a warning that we need to call prepare to play.
    jassert(isPrepared);

    // Create an Audioblock of type float out of the Audio Buffer
    juce::dsp::AudioBlock<float> block{ buffer };

    // Process with Context Replacing the Audio Block
    filter.process(juce::dsp::ProcessContextReplacing<float>{ block });
}
void FilterData::updateParameters(const int filterType, const float frequency, const float resonance, const float modulator = 1.0f)
{
    // Sets the type of filter, LP, BP, or HP
    switch (filterType)
    {
    case 0:
        filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
        break;
    case 1:
        filter.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
        break;
    case 2:
        filter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
        break;
    }

    // The cutoff frequency is multiplied by the modulation amount.
    float modFreq = frequency * modulator;

    // Make sure the modFreq is between 20.0f and 20000.0f
    modFreq = std::fmax(modFreq, 20.0f);
    modFreq = std::fmin(modFreq, 20000.0f);

    //filter.setCutoffFrequency(frequency);;

    // Set the Cuttoff Frequency
    filter.setCutoffFrequency(modFreq);

    // Set the Resonance amount
    filter.setResonance(resonance);
}
void FilterData::reset()
{
    // Reset the filter.
    filter.reset();
}