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
    // Since we are within the Oscillator Class we just need to call prepare with the ProcessSpec reference
    prepare(spec);
}

void OscData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block)
{
    // Call the process the next audio block replacing the data in it.
    process(juce::dsp::ProcessContextReplacing<float>(block));
}

void OscData::setWaveFrequency(const int midiNoteNumber)
{
    // Set the frequency of the midi note after converting it to Hertz
    setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
}

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