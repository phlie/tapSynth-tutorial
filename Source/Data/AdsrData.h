/*
  ==============================================================================

    AdsrData.h
    Created: 6 Sep 2022 9:14:58am
    Author:  phlie

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

// This class is used for the data for the ADSR envelope.
class AdsrData : public juce::ADSR
{
public:
    // Used to update the ADSR when called
    void updateADSR(const float attack, const float decay, const float sustain, const float release);

private:
    // A very simple ADSR envelope class.
    juce::ADSR adsr;
    // Holds the paramters being used by an ADSR object
    juce::ADSR::Parameters adsrParams;
};
