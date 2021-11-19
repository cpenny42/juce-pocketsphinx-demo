/*
  ==============================================================================

    Decoder.cpp
    Created: 18 Nov 2021 9:10:08pm
    Author:  Chris Penny

  ==============================================================================
*/

#include "Decoder.h"
#include <pocketsphinx.h>
using namespace juce;

//    const auto MODELDIR =
//    File::getSpecialLocation(File::SpecialLocationType::currentApplicationFile)
//        .getChildFile("Contents")
//        .getChildFile("Resources")
//        .getChildFile("model")
//        .getFullPathName();
//
//    const auto hmm = String(MODELDIR + "/en-us/en-us").toRawUTF8();
//    const auto lm = String(MODELDIR + "/en-us/en-us.lm.dmp").toRawUTF8();
//    const auto dict = String(MODELDIR +
//    "/en-us/cmudict-en-us.dict").toRawUTF8(); const auto goforward =
//    String(MODELDIR + "/en-us/goforward.raw").toRawUTF8();

const auto hmm = "/Users/chrispenny/Developer/personal/voice-to-csv/app/lib/"
                 "share/pocketsphinx/model/en-us/en-us";
const auto lm = "/Users/chrispenny/Developer/personal/voice-to-csv/app/lib/"
                "share/pocketsphinx/model/en-us/en-us.lm.dmp";
const auto dict = "/Users/chrispenny/Developer/personal/voice-to-csv/app/lib/"
                  "share/pocketsphinx/model/en-us/cmudict-en-us.dict";
const auto goforward = "/Users/chrispenny/Developer/personal/voice-to-csv/"
                       "app/lib/share/pocketsphinx/model/en-us/goforward.raw";

Decoder::Decoder()
    : failed (false), utt_started (false), in_speech (false)
{
}

Decoder::~Decoder()
{
}

const StringArray& Decoder::getUtterances()
{
    const juce::SpinLock::ScopedLockType sl (lock);
    utterances = StringArray (_utterances);
    _utterances.clearQuick();
    return utterances;
}

void Decoder::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    failed = false;
    config = cmd_ln_init (NULL, ps_args(), TRUE, "-hmm", hmm, "-lm", lm, "-dict", dict, "-samprate", String(sampleRate).toRawUTF8(), "-nfft", "4096", NULL);
    if (config == NULL)
    {
        fprintf (stderr, "Failed to create config object, see log for details\n");
        failed = true;
        return;
    }

    ps = ps_init (config);
    if (ps == NULL)
    {
        fprintf (stderr, "Failed to create recognizer, see log for details\n");
        failed = true;
        return;
    }
    rv = ps_start_utt (ps);
    bridge = new int16[samplesPerBlockExpected];
    utt_started = false;
    in_speech = false;
}

void Decoder::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    if (failed == true)
    {
        return;
    }
    auto buffer = bufferToFill.buffer;
    const auto numSamples = buffer->getNumSamples();
    AudioDataConverters::convertFloatToInt16LE (buffer->getReadPointer (0), bridge, numSamples, 2);
    ps_process_raw (ps, bridge, numSamples, false, false);
    in_speech = ps_get_in_speech (ps);
    if (in_speech && ! utt_started)
    {
        utt_started = TRUE;
        printf ("Listening...\n");
    }
    if (! in_speech && utt_started)
    {
        /* speech -> silence transition, time to start new utterance  */
        ps_end_utt (ps);
        hyp = ps_get_hyp (ps, NULL);
        if (hyp != NULL)
        {
            {
                const juce::SpinLock::ScopedLockType sl (lock);
                _utterances.add (String (hyp));
            }
            printf ("%s\n", hyp);
            fflush (stdout);
        }

        if (ps_start_utt (ps) < 0)
            printf ("Failed to start utterance\n");
        utt_started = FALSE;
        printf ("Ready....\n");
    }
}

void Decoder::releaseResources()
{
    rv = ps_end_utt (ps);
    hyp = ps_get_hyp (ps, &score);
    printf ("Recognized: %s\n", hyp);
    ps_free (ps);
    cmd_ln_free_r (config);
    free (bridge);
}
