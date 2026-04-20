#pragma once
#include <JuceHeader.h>

class MainComponent : public juce::AudioAppComponent,
    public juce::MidiInputCallback,
    public juce::Timer

{
public:
    MainComponent();
    ~MainComponent() override;


    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void handleIncomingMidiMessage(juce::MidiInput*,
        const juce::MidiMessage& message) override;

   

    void loadSample(juce::File file);



    int panelX = 1000; // start off screen

    juce::dsp::Reverb reverb;

    juce::TextButton audioSettingsButton{ "Sound Options" };

    juce::AudioDeviceSelectorComponent deviceSelector{
    deviceManager,
    0, 2,
    0, 2,
    true, true, true, false
    };
    bool showAudioPanel = false;

    
    void MainComponent::timerCallback()
    {
        repaint();

        int targetX = showAudioPanel ? getWidth() - 300 : getWidth();

        panelX += (targetX - panelX) * 0.2f;

        deviceSelector.setTopLeftPosition(panelX, 50);
    }

    juce::Label sampleNameLabel;

private:


    // ================= MIDI Keyboard =================
    juce::MidiKeyboardState keyboardState;
    juce::MidiKeyboardComponent keyboardComponent{
        keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard
    };

    // ================= Oscillator =================
    juce::dsp::Oscillator<float> osc{ [](float x) { return std::sin(x); } };

    // ================= LFO (Vibrato) =================
    juce::dsp::Oscillator<float> lfo{ [](float x) { return std::sin(x); } };


    // Breath noise
    juce::Random random;

    // ================= Low Pass Filter =================
    juce::dsp::StateVariableTPTFilter<float> lowPass;


    // ================= ADSR =================
    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;


    // ===== Preset Buttons =====
    juce::TextButton savePresetButton;
    juce::TextButton loadPresetButton;




    float noteVelocity = 1.0f; // 0.0 – 1.0


    juce::Synthesiser sampler;
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    std::unique_ptr<juce::AudioFormatReader> reader;

    // ================= Metronome  =================

    juce::ComboBox beatSelector;
    int beatsPerBar = 4;
    int currentBeat = 0;

    juce::ToggleButton metronomeButton{ "Metronome" };
    juce::Slider bpmSlider;

    double currentSampleRate = 44100.0;
    int samplesUntilNextTick = 0;

    // ================= Recode button =================

   

   

    juce::TextButton backButton{ "Back" };

    juce::TextButton recordStartButton{ "Start" };
    juce::TextButton recordStopButton{ "Stop" };
    juce::TextButton playMidiButton{ "Play" };


    void setMainUIVisible(bool visible);

    // ================= State =================
    bool noteIsActive = false;
    float baseFrequency = 440.0f;
    float vibratoDepth = 5.0f;   // Hz
    float breathAmount = 0.02f;  // noise level

    // ================= UI Sliders =================
    juce::Slider attackSlider;
    juce::Slider releaseSlider;
    juce::Slider cutoffSlider;
    juce::Slider vibratoSlider;
    juce::Slider breathSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;

    juce::SmoothedValue<float> smoothCutoff;
    juce::SmoothedValue<float> smoothVibrato;
    juce::SmoothedValue<float> smoothBreath;



    // UI Labels
    juce::Label attackLabel;
    juce::Label decayLabel;
    juce::Label sustainLabel;
    juce::Label releaseLabel;
    juce::Label cutoffLabel;
    juce::Label vibratoLabel;
    juce::Label breathLabel;

    float leftLevel = 0.0f;
    float rightLevel = 0.0f;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)


};
