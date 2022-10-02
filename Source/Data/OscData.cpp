/*
  ==============================================================================

    OscData.cpp
    Created: 18 Sep 2022 11:47:26am
    Author:  phlie

  ==============================================================================
*/

#include "OscData.h"

void OscData::prepareToPlay(juce::dsp::ProcessSpec& spec)
{
    // Prepare the fmOsc for play
    fmOsc.prepare(spec);

    // Since we are within the Oscillator Class we just need to call prepare with the ProcessSpec reference
    prepare(spec);
}

void OscData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block)
{
    // Loop through all the channels
    for (int ch = 0; ch < block.getNumChannels(); ++ch)
    {
        // For each sample in the block
        for (int s = 0; s < block.getNumSamples(); ++s)
        {
            // Set fmMod to be the processesd current sample times the fmDepth
            fmMod = fmOsc.processSample(block.getSample(ch, s)) * fmDepth;
        }
    }

    // Call the process the next audio block replacing the data in it.
    process(juce::dsp::ProcessContextReplacing<float>(block));
}

// [[[ERROR]]] The Frequency Modulation is only being updated once per block when it should be once per sample.
void OscData::setWaveFrequency(const int midiNoteNumber)
{
    // Set the frequency of the midi note after converting it to Hertz + the modification of the FM Osc
    setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) + fmMod);

    // Set the last midi note.
    lastMidiNote = midiNoteNumber;
}

// Choose the wave type for the osc.
void OscData::setWaveType(const int choice)
{
    //std::sin(x); // Sine Wave
    // x / MathContants<float>::pi;
    // x < 0.0f ? -1.0 : 1.0
    switch (choice)
    {
        case 0:
            // Sine wave
            initialise([](float x) {return std::sin(x);  });
            break;
        case 1:
            // Saw wave
            initialise([](float x) {return x / juce::MathConstants<float>::pi; });
            break;
        case 2:
            // Square wave
            initialise([](float x) {return x < 0.0f ? -1.0f : 1.0f;  });
            break;
        default:
            // In case there is some kind of failure, fail and exit.
            jassertfalse;   // You're not suppose to be here!
            break;
    }
}

void OscData::setFmParams(const float depth, const float freq)
{
    // Set the frequency and depth
    fmOsc.setFrequency(freq);
    fmDepth = depth;

    // Sets the current frequency of the osc.
    auto currentFreq = juce::MidiMessage::getMidiNoteInHertz(lastMidiNote) + fmMod;

    // Set the frequency of the original osc making sure it is always positive.
    setFrequency(currentFreq >= 0 ? currentFreq : currentFreq * -1.0f);
}