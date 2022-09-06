/*
  ==============================================================================

    SynthVoice.cpp
    Created: 29 Aug 2022 7:48:17pm
    Author:  phlie

  ==============================================================================
*/

#include "SynthVoice.h"

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    // If sound can be dynamically casted as a SynthesiserSound, meaning it is derived from SynthesiserSound.
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    // Set the frequency of the midi note after converting it to Hertz
    sawWave.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    // Starts the envelope.
    adsr.noteOn();
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    // Signals the adsr to start the release stage
    adsr.noteOff();

    // If no more tail off or the adsr is not active, clear current note.
    if (!allowTailOff || !adsr.isActive())
    {
        clearCurrentNote();
    }
}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{
}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    // Tell the adsr the current sample rate.
    adsr.setSampleRate(sampleRate);

    // This structure is passed into a DSP algorithm's prepare() method, and contains information about various aspects of the context in which it can expect to be called.
    juce::dsp::ProcessSpec spec;
    // Set the largest block size
    spec.maximumBlockSize = samplesPerBlock;
    // Set the sample rate
    spec.sampleRate = sampleRate;
    // Set the total number of channels
    spec.numChannels = outputChannels;

    // Tell the sawWave osciallator to get prepared with the ProcessSpec's
    sawWave.prepare(spec);

    // Tell the gain to do as the sawWave did.
    gain.prepare(spec);

    //sawWave.setFrequency(220.0f);
    // 
    // Set the current amount of gain.
    gain.setGainLinear(0.5f);



    // Signal that prepare to play has been called.
    isPrepared = true;
}

// Called from PluginProcessor to update the ADSR with atomic float variables when they are changed.
void SynthVoice::updateADSR(const float attack, const float decay, const float sustain, const float release)
{
    // Give values to all the adsr parameters for now
    adsrParams.attack = attack;
    adsrParams.decay = decay;
    adsrParams.sustain = sustain;
    adsrParams.release = release;

    // Set the adsr parameters
    adsr.setParameters(adsrParams);
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    // Abort and throw an error unless the bool isPrepared is set and startup has run
    jassert(isPrepared);

    // For the case that the synth is not currently active.
    if (!isVoiceActive())
        return;

    // Set the size for synthBuffer while not reallocating memory unless it needs to grow.
    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    synthBuffer.clear();

    /*juce::dsp::AudioBlock<float> audioBlock{outputBuffer};
    sawWave.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    
    adsr.applyEnvelopeToBuffer(outputBuffer, startSample, numSamples); */

    // Necessary so that two notes overlapping don't cause phase issues.
    juce::dsp::AudioBlock<float> audioBlock{ synthBuffer };
    sawWave.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    // Apply the envelope to the synthBuffer from 0, to its total number of samples.
    adsr.applyEnvelopeToBuffer(synthBuffer, 0,synthBuffer.getNumSamples());

    // Can be used for debugging
    //if (startSample != 0)
    //     jassertfalse;

    // Loop through each channel.
    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
    {
        // adds samples from synthBuffer to outputBuffer
        outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);

        // If the adsr is no longer active clear the note.
        if (!adsr.isActive())
        {
            clearCurrentNote();
        }
    }
}
