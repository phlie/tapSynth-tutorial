/*
  ==============================================================================

    OscData.h
    Created: 18 Sep 2022 11:47:26am
    Author:  phlie

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

// The OscData class inherits from juce dsp Oscillator with type float.
class OscData : public juce::dsp::Oscillator<float>
{
public:
    // Two standard functions for default behavior one for preparing to play and another to generate the audio data
    void prepareToPlay(juce::dsp::ProcessSpec& spec);

    // Process the next audio block.
    void getNextAudioBlock(juce::dsp::AudioBlock<float>& block);

    // Used to set the frequency of the current note.
    void setWaveFrequency(const int midiNoteNumber);

    // Chooses between the different wave types
    void setWaveType(const int choice);

    // Sets the Frequency modulation parameters.
    void setFmParams(const float depth, const float freq);

private:
    // A juce DSP oscillator of type float which defaults to a sin wave.
    juce::dsp::Oscillator<float> fmOsc{ [](float x) { return std::sin(x);  } };

    // The variable to hold the frequency modulation amount.
    float fmMod{ 0.0f };

    // The variable to hold the fmDepth
    float fmDepth{ 0.0f };

    // Last Midi Note Played.
    int lastMidiNote{ 0 };
};