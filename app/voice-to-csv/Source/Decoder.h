/*
  ==============================================================================

    Decoder.h
    Created: 18 Nov 2021 9:10:08pm
    Author:  Chris Penny

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

typedef struct ps_decoder_s ps_decoder_t;
typedef struct cmd_ln_s cmd_ln_t;

class Decoder  : public juce::AudioSource
{
public:
    //==============================================================================
    Decoder();
    ~Decoder() override;

    const juce::StringArray& getUtterances();

    // AudioSource
    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

private:
    ps_decoder_t* ps;
    cmd_ln_t* config;
    char const* hyp;
    int rv;
    juce::int32 score;
    bool failed;
    bool utt_started;
    bool in_speech;
    juce::int16* bridge;
    juce::StringArray _utterances, utterances;
    juce::SpinLock lock;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Decoder)
};
