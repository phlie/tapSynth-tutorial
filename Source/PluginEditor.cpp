/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TapSynthAudioProcessorEditor::TapSynthAudioProcessorEditor(TapSynthAudioProcessor& p)
    : AudioProcessorEditor(&p)
    , audioProcessor(p)
    , osc(audioProcessor.apvts, "OSC1WAVETYPE", "FMFREQ", "FMDEPTH")
    , adsr("Amp Envelope", audioProcessor.apvts, "ATTACK", "DECAY", "SUSTAIN", "RELEASE")
    , filter(audioProcessor.apvts, "FILTERTYPE", "FILTERCUTOFF", "FILTERRES")
    , modAdsr("Mod Envelope", audioProcessor.apvts, "MODATTACK", "MODDECAY", "MODSUSTAIN", "MODRELEASE")
{
    auto logo = juce::ImageCache::getFromMemory(BinaryData::background_jpg, BinaryData::background_jpgSize);

    // If the image has been loaded from memory
    if (!logo.isNull())
    {
        // Set the image and set it to stretch to fill its bounds
        backgroundImageComponent.setImage(logo, juce::RectanglePlacement::stretchToFit);
    }
    else
    {
        // If the image is not loaded quit because of error.
        jassert(!logo.isNull());
    }

    addAndMakeVisible(backgroundImageComponent);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 500);

    // Add and make visible our two custom classes
    addAndMakeVisible(osc);
    addAndMakeVisible(adsr);
    addAndMakeVisible(filter);
    addAndMakeVisible(modAdsr);
}

TapSynthAudioProcessorEditor::~TapSynthAudioProcessorEditor()
{
}

//==============================================================================
void TapSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Make the background black
    g.fillAll(juce::Colours::black);
}

void TapSynthAudioProcessorEditor::resized()
{
    // osc resize
    //osc.setBounds(10, 10, 200, 200);
    // adsr resize.
    //adsr.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight());
    const auto paddingX = 5;
    const auto paddingY = 30;

    osc.setBounds(paddingX, paddingY, 300, 200);
    adsr.setBounds(osc.getRight() + 2*paddingX, paddingY, 280, 200);
    filter.setBounds(paddingX, osc.getBottom() + paddingY, 300, 200);
    modAdsr.setBounds(filter.getRight() + 2 * paddingX, osc.getBottom() + paddingY, 280, 200);

    // Set the relative bounds of the image component.
    backgroundImageComponent.setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);
}


