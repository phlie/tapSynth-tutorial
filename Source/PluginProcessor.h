/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "SynthVoice.h"
#include "Data/FilterData.h"

//==============================================================================
/**
*/
class TapSynthAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    TapSynthAudioProcessor();
    ~TapSynthAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // A class to contain a set of RangedAudioParameters and AudioProcessorParameterGroups containing RangedAudioParameters.
    juce::AudioProcessorValueTreeState apvts;

private:
    // Base class for a musical device that can play sounds.
    // To create a synthesiser, you'll need to create a subclass of SynthesiserSound to describe each sound available to your synth, and a subclass of SytnhesiserVoice which can play back on of those sounds.
    juce::Synthesiser synth;
    
    FilterData filter;

    // This class is used in the AudioProcessorValueTreeState constructor to allow arbitrarily grouped RangedAudioParameters to be passed to an AudioProcessor.
    juce::AudioProcessorValueTreeState::ParameterLayout createParams();
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TapSynthAudioProcessor)
};
