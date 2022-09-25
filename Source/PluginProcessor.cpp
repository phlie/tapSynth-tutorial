/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TapSynthAudioProcessor::TapSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
         // Construct an instance of the AudioProcessorValueTreeState with these settings and call createParams()
                       ), apvts(*this, nullptr, "Parameters", createParams())
#endif
{
    // Add the Synth Voice and Synth Sound user created classes as the voice and sound for the synthesiser class.
    synth.addSound(new SynthSound());
    synth.addVoice(new SynthVoice());
}

TapSynthAudioProcessor::~TapSynthAudioProcessor()
{
}

//==============================================================================
const juce::String TapSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TapSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TapSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TapSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TapSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TapSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TapSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TapSynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TapSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void TapSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void TapSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    // Sets the sample rate as the value passed in from when playing the timeline.
    synth.setCurrentPlaybackSampleRate(sampleRate);

    // Loop through the total amount of voices
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        // If it can be casted to SynthVoice*
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            // Tell that voice to get ready to play
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }

    }
}

void TapSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TapSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void TapSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // Default processBlock stuff
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    /*for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    } */

    // This loop will later on hold information upon a voice being succussfully cast to SynthesiserVoice
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        // Changed to see if it is our own synth voice class not the default one so we can call updateADSR
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            // Osc controls
            // ADSR
            // Get the value of the parameter with a certain codename
               
            // Used to get the choice of oscillator
            auto& oscWaveChoice = *apvts.getRawParameterValue("OSC1WAVETYPE");

            // Used for FM Synthesis
            auto& fmDepth = *apvts.getRawParameterValue("FMDEPTH");
            auto& fmFreq = *apvts.getRawParameterValue("FMFREQ");

            // Amp Envelope
            auto& attack = *apvts.getRawParameterValue("ATTACK");
            auto& decay = *apvts.getRawParameterValue("DECAY");
            auto& sustain = *apvts.getRawParameterValue("SUSTAIN");
            auto& release = *apvts.getRawParameterValue("RELEASE");

            // Filter Section
            auto& filterType = *apvts.getRawParameterValue("FILTERTYPE");
            auto& cutoff = *apvts.getRawParameterValue("FILTERCUTOFF");
            auto& resonance = *apvts.getRawParameterValue("FILTERRES");

            // Used for modulation
            auto& modAttack = *apvts.getRawParameterValue("MODATTACK");
            auto& modDecay = *apvts.getRawParameterValue("MODDECAY");
            auto& modSustain = *apvts.getRawParameterValue("MODSUSTAIN");
            auto& modRelease = *apvts.getRawParameterValue("MODRELEASE");

            voice->getOscillator().setWaveType(oscWaveChoice);
            voice->getOscillator().setFmParams(fmDepth, fmFreq);
            // The .load() on the end indicates that it is an atomic float not a normal float.
            // Updated to new class that encapsalates the Adsr component and data.
            voice->updateAdsr(attack.load(), decay.load(), sustain.load(), release.load());
            voice->updateFilter(filterType.load(), cutoff.load(), resonance.load());
            voice->updateModAdsr(modAttack.load(), modDecay.load(), modSustain.load(), modRelease.load());
            // LFO
        }
    }

    // For now, when a midi note is pressed, log the message with a timestamp
    for (const juce::MidiMessageMetadata metadata : midiMessages)
        if (metadata.numBytes == 3)
            juce::Logger::writeToLog("TimeStamp: " + juce::String(metadata.getMessage().getTimeStamp()));

    // Call the synth renderNextBlock with the current buffer, any midiMessages, start sample in the buffer, and total number of samples in the buffer.
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());



    //filter.process(buffer);
}

//==============================================================================
bool TapSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TapSynthAudioProcessor::createEditor()
{
    return new TapSynthAudioProcessorEditor (*this);
}

//==============================================================================
void TapSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TapSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TapSynthAudioProcessor();
}

// Value Tree
juce::AudioProcessorValueTreeState::ParameterLayout TapSynthAudioProcessor::createParams()
{
    // Combobox: switch oscillator
    // Attack - float
    // Decay - float
    // Sustain - float
    // Release - float

    // Create a vector of unique_ptr's all of type RangedAudioParameter called params.
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // OSC select
    // Add a AudioParameterChoice to the unique_ptr array of RangedAudioParameter's called params.
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC", "Osclillator", juce::StringArray{ "Sine", "Saw", "Square" }, 0));

    // ADSR
    // Instead of adding a choice, add a float with its code name, written name, range, and default value.
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", juce::NormalisableRange<float> {0.1f, 1.0f}, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", juce::NormalisableRange<float> {0.1f, 1.0f}, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", juce::NormalisableRange<float> {0.1f, 1.0f}, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", juce::NormalisableRange<float> {0.1f, 3.0f}, 0.1f));

    // OSC 1 Wave Type Choice Selection
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC1WAVETYPE", "Osc 1 Wave Type", juce::StringArray{
        "Sine", "Saw", "Square" }, 0));

    // The two parameters for FM Synthesis
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FMFREQ", "FM Frequency", juce::NormalisableRange<float> {0.0f, 1000.0f, 0.01f, 0.3f}, 0.0f));
    // Increment by 0.01 and set the skew factor as 0.3 which gives the lower range more of the room on the slider.
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FMDEPTH", "FM Depth", juce::NormalisableRange<float> {0.0f, 1000.0f, 0.01f, 0.3f}, 0.0f));

    // Filter
    params.push_back(std::make_unique<juce::AudioParameterChoice>("FILTERTYPE", "Filter Type", juce::StringArray{ "low-pass", "band-pass", "high-pass" }, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTERCUTOFF", "Filter Cutoff", juce::NormalisableRange<float>{20.0f, 20000.0f, 0.1f, 0.6f}, 200.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTERRES", "Filter Resonance", juce::NormalisableRange<float>{1.0f, 10.0f, 0.1f}, 1.0f));

    // Modulation ADSR
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MODATTACK", "Mod Attack", juce::NormalisableRange<float> {0.1f, 1.0f}, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MODDECAY", "Mod Decay", juce::NormalisableRange<float> {0.1f, 1.0f}, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MODSUSTAIN", "Mod Sustain", juce::NormalisableRange<float> {0.1f, 1.0f}, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MODRELEASE", "Mod Release", juce::NormalisableRange<float> {0.1f, 3.0f}, 0.1f));
    // Finally return where the unique vectors start and end on params.
    return { params.begin(), params.end() };
}