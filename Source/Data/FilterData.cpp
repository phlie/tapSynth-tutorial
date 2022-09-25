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
    filter.reset();

    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = numChannels;
    filter.prepare(spec);

    isPrepared = true;
}
void FilterData::process(juce::AudioBuffer<float>& buffer)
{
    // If this is not true, it will stop and throw a warning that we need to call prepare to play.
    jassert(isPrepared);
    juce::dsp::AudioBlock<float> block{ buffer };
    filter.process(juce::dsp::ProcessContextReplacing<float>{ block });
}
void FilterData::updateParameters(const int filterType, const float frequency, const float resonance, const float modulator = 1.0f)
{
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

    float modFreq = frequency * modulator;

    modFreq = std::fmax(modFreq, 20.0f);
    modFreq = std::fmin(modFreq, 20000.0f);

    //filter.setCutoffFrequency(frequency);;
    filter.setCutoffFrequency(modFreq);
    filter.setResonance(resonance);
}
void FilterData::reset()
{
    filter.reset();
}