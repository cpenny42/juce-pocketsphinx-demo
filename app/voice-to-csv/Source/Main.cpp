/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include "MainComponent.h"
#include <JuceHeader.h>
//#include <pocketsphinx.h>
using namespace juce;

//int initialize_pocketsphinx()
//{
//    //    const auto MODELDIR =
//    //    File::getSpecialLocation(File::SpecialLocationType::currentApplicationFile)
//    //        .getChildFile("Contents")
//    //        .getChildFile("Resources")
//    //        .getChildFile("model")
//    //        .getFullPathName();
//    //
//    //    const auto hmm = String(MODELDIR + "/en-us/en-us").toRawUTF8();
//    //    const auto lm = String(MODELDIR + "/en-us/en-us.lm.dmp").toRawUTF8();
//    //    const auto dict = String(MODELDIR +
//    //    "/en-us/cmudict-en-us.dict").toRawUTF8(); const auto goforward =
//    //    String(MODELDIR + "/en-us/goforward.raw").toRawUTF8();
//
//    const auto hmm = "/Users/chrispenny/Developer/personal/voice-to-csv/app/lib/"
//                     "share/pocketsphinx/model/en-us/en-us";
//    const auto lm = "/Users/chrispenny/Developer/personal/voice-to-csv/app/lib/"
//                    "share/pocketsphinx/model/en-us/en-us.lm.dmp";
//    const auto dict = "/Users/chrispenny/Developer/personal/voice-to-csv/app/lib/"
//                      "share/pocketsphinx/model/en-us/cmudict-en-us.dict";
//    const auto goforward = "/Users/chrispenny/Developer/personal/voice-to-csv/"
//                           "app/lib/share/pocketsphinx/model/en-us/goforward.raw";
//
//    ps_decoder_t* ps;
//    cmd_ln_t* config;
//    FILE* fh;
//    char const *hyp, *uttid;
//    int16 buf[512];
//    int rv;
//    int32 score;
//
//    config = cmd_ln_init (NULL, ps_args(), TRUE, "-hmm", hmm, "-lm", lm, "-dict", dict, NULL);
//    if (config == NULL)
//    {
//        fprintf (stderr, "Failed to create config object, see log for details\n");
//        return -1;
//    }
//
//    ps = ps_init (config);
//    if (ps == NULL)
//    {
//        fprintf (stderr, "Failed to create recognizer, see log for details\n");
//        return -1;
//    }
//
//    fh = fopen (goforward, "rb");
//    if (fh == NULL)
//    {
//        fprintf (stderr, "Unable to open input file goforward.raw\n");
//        return -1;
//    }
//
//    rv = ps_start_utt (ps);
//
//    while (! feof (fh))
//    {
//        size_t nsamp;
//        nsamp = fread (buf, 2, 512, fh);
//        rv = ps_process_raw (ps, buf, nsamp, FALSE, FALSE);
//    }
//
//    rv = ps_end_utt (ps);
//    hyp = ps_get_hyp (ps, &score);
//    printf ("Recognized: %s\n", hyp);
//
//    fclose (fh);
//    ps_free (ps);
//    cmd_ln_free_r (config);
//
//    return 0;
//}

//==============================================================================
class voicetocsvApplication  : public juce::JUCEApplication
{
public:
    //==============================================================================
    voicetocsvApplication() {}

    const juce::String getApplicationName() override
    {
        return ProjectInfo::projectName;
    }
    const juce::String getApplicationVersion() override
    {
        return ProjectInfo::versionString;
    }
    bool moreThanOneInstanceAllowed() override { return true; }

    //==============================================================================
    void initialise (const juce::String& commandLine) override
    {
        // This method is where you should put your application's initialisation
        // code..

        //        initialize_pocketsphinx();

        mainWindow.reset (new MainWindow (getApplicationName()));
    }

    void shutdown() override
    {
        // Add your application's shutdown code here..

        mainWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app
        // to close.
        quit();
    }

    void anotherInstanceStarted (const juce::String& commandLine) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }

    //==============================================================================
    /*
      This class implements the desktop window that contains an instance of
      our MainComponent class.
  */
    class MainWindow  : public juce::DocumentWindow
    {
    public:
        MainWindow (juce::String name)
            : DocumentWindow (
                name,
                juce::Desktop::getInstance().getDefaultLookAndFeel().findColour (
                    juce::ResizableWindow::backgroundColourId),
                DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            setContentOwned (new MainComponent(), true);

#if JUCE_IOS || JUCE_ANDROID
            setFullScreen (true);
#else
            setResizable (true, true);
            centreWithSize (getWidth(), getHeight());
#endif

            setVisible (true);
        }

        void closeButtonPressed() override
        {
            // This is called when the user tries to close this window. Here, we'll
            // just ask the app to quit when this happens, but you can change this to
            // do whatever you need.
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
       class uses a lot of them, so by overriding you might break its
       functionality. It's best to do all your work in your content component
       instead, but if you really have to override any DocumentWindow methods,
       make sure your subclass also calls the superclass's method.
    */

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (voicetocsvApplication)
