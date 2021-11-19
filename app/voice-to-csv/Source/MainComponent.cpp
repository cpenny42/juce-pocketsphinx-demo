#include "MainComponent.h"
using namespace juce;

//==============================================================================
MainComponent::MainComponent()
    : selector (deviceManager, 1, 1, 2, 2, false, false, true, false)
{
    addAndMakeVisible (&selector);
    addAndMakeVisible (&liveAudioScroller);
    addAndMakeVisible (&textEditor);

    textEditor.setReadOnly (true);
    textEditor.setMultiLine (true);
    textEditor.setCaretVisible (false);

    deviceManager.addAudioCallback (&liveAudioScroller);
    setSize (800, 600);

    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (
            RuntimePermissions::recordAudio,
            [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        setAudioChannels (2, 2);
    }
    startTimerHz (5);
}

MainComponent::~MainComponent()
{
    stopTimer();
    deviceManager.removeAudioCallback (&liveAudioScroller);
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    decoder.prepareToPlay (samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    decoder.getNextAudioBlock (bufferToFill);

    // We need to clear the output buffers before returning, in case they're
    // full of junk..
    for (int j = 0; j < bufferToFill.buffer->getNumChannels(); ++j)
        if (float* outputChannel = bufferToFill.buffer->getWritePointer (j))
            juce::zeromem (outputChannel, (size_t) bufferToFill.buffer->getNumSamples() * sizeof (float));
}

void MainComponent::releaseResources()
{
    decoder.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
}

void MainComponent::resized()
{
    liveAudioScroller.setBounds (0, 0, getWidth(), 200);
    textEditor.setBounds (0, 200, proportionOfWidth (1), getHeight() - 400);
    selector.setBounds (0, getHeight() - 200, getWidth(), 200);
}

//==============================================================================
void MainComponent::timerCallback()
{
    utterances = decoder.getUtterances();
    const String text = utterances.joinIntoString (" ");
    const String suffix = text.trim().length() > 0 ? " " : "";
    textEditor.setCaretPosition (textEditor.getTotalNumChars());
    textEditor.insertTextAtCaret (text + suffix);
}
