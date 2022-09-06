/*
  ==============================================================================

    AdsrData.cpp
    Created: 6 Sep 2022 9:14:58am
    Author:  phlie

  ==============================================================================
*/

#include "AdsrData.h"

// Called from PluginProcessor to update the ADSR with atomic float variables when they are changed.
void AdsrData::updateADSR(const float attack, const float decay, const float sustain, const float release)
{
    // Give values to all the adsr parameters for now
    adsrParams.attack = attack;
    adsrParams.decay = decay;
    adsrParams.sustain = sustain;
    adsrParams.release = release;

    // Set the adsr parameters
    setParameters(adsrParams);
}