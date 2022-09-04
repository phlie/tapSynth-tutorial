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
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
private:
    // A very simple ADSR envelope class.
    juce::ADSR adsr;
    // Holds the paramters being used by an ADSR object
    juce::ADSR::Parameters adsrParams;
    // A multi-channel buffer containing floating point audio samples.
    juce::AudioBuffer<float> synthBuffer;

    // Generates a signal based on a user-supplied function
    juce::dsp::Oscillator<float> sineWave{ [](float x) {return std::sin(x); } };
    // With a lookup table of 200 points
    juce::dsp::Oscillator<float> sawWave{ [](float x) { return x / juce::MathConstants<float>::pi; }, 200 };
    juce::dsp::Oscillator<float> squareWave{ [](float x) { return x < 0.0f ? -1.0f : 1.0f; } };
    
    // Applies a gain to audio samples as single samples or AudioBlocks.
    juce::dsp::Gain<float> gain;

    // Used to inform renderNextBlock that prepareToPlay has already been called.
    bool isPrepared = false;
};