/*
  ==============================================================================

    SynthSound.h
    Created: 29 Aug 2022 7:48:36pm
    Author:  phlie

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

// A class that describes one of the sounds that a Synthesiser can play.
// The SynthesiserSound is a passive class that just describes what the sound is - the actual audio rendering for a sound is done by a SynthesiserVoice.
// This allows more than one SynthesiserVoice to play the same sound at the same time.
class SynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote(int midiNoteNumber) override { return true; }
    bool appliesToChannel(int midiChannel) override { return true; }
};