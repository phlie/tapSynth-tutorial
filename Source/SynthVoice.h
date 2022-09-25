/*
  ==============================================================================

    SynthVoice.h
    Created: 29 Aug 2022 7:48:17pm
    Author:  phlie

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "Data/OscData.h"
#include "Data/AdsrData.h"
#include "Data/FilterData.h"

// Represents a voice that a Synthesiser can use to play a SynthesiserSound.
// A voice plays a single sound at a time, and a synthesiser holds an array of of vocies so that it can play polyphonically.
class SynthVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannel);

    void updateAdsr(const float attack, const float decay, const float sustain, const float release);
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

    void updateFilter(const int filterType, const float cutoff, const float resonance);
    void updateModAdsr(const float attack, const float decay, const float sustain, const float release);
    // 
    OscData& getOscillator() { return osc; }
private:

    // A multi-channel buffer containing floating point audio samples.
    juce::AudioBuffer<float> synthBuffer;

    // Our own oscillator class
    OscData osc;

    // Our own adsr class
    AdsrData adsr;
    
    // Can be implemented either in AudioProcessor class or the individual SynthVoice class
    FilterData filter;

    AdsrData modAdsr;
    // Generates a signal based on a user-supplied function
    //juce::dsp::Oscillator<float> sineWave{ [](float x) {return std::sin(x); } };
    // With a lookup table of 200 points
    //juce::dsp::Oscillator<float> sawWave{ [](float x) { return x / juce::MathConstants<float>::pi; }, 200 };
    //juce::dsp::Oscillator<float> squareWave{ [](float x) { return x < 0.0f ? -1.0f : 1.0f; } };
    
    // Applies a gain to audio samples as single samples or AudioBlocks.
    juce::dsp::Gain<float> gain;

    // Used to inform renderNextBlock that prepareToPlay has already been called.
    bool isPrepared = false;
};