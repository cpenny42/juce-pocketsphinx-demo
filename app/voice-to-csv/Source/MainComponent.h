#pragma once
#include "Decoder.h"
#include "LiveScrollingAudioDisplay.h"
#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent, private juce::Timer
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    void timerCallback() override;

    //==============================================================================
    // Your private member variables go here...
    juce::AudioDeviceSelectorComponent selector;
    LiveScrollingAudioDisplay liveAudioScroller;
    Decoder decoder;
    juce::TextEditor textEditor;
    juce::StringArray utterances;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
