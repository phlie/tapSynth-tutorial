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
    void getNextAudioBlock(juce::dsp::AudioBlock<float>& block);

    // Used to set the frequency of the current note.
    void setWaveFrequency(const int midiNoteNumber);

    // Chooses between the different wave types
    void setWaveType(const int choice);

private:


};