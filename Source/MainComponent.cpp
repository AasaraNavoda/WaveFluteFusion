#include "MainComponent.h"

// ==================================================
MainComponent::MainComponent()
{
    setSize(1000, 550);

    setAudioChannels(0, 2); // stereo out

    // ---------- Cutoff Slider ----------
    cutoffSlider.setSliderStyle(juce::Slider::Rotary);
    cutoffSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    cutoffSlider.setRange(500.0, 6000.0);
    cutoffSlider.setValue(2000.0);
    addAndMakeVisible(cutoffSlider);

    // ---------- Vibrato Slider ----------
    vibratoSlider.setSliderStyle(juce::Slider::Rotary);
    vibratoSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    vibratoSlider.setRange(0.0, 10.0);
    vibratoSlider.setValue(3.0);
    addAndMakeVisible(vibratoSlider);

    // ---------- Breath Slider ----------
    breathSlider.setSliderStyle(juce::Slider::Rotary);
    breathSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    breathSlider.setRange(0.0, 0.2);
    breathSlider.setValue(0.05);
    addAndMakeVisible(breathSlider);

    // Sustain slider
    sustainSlider.setSliderStyle(juce::Slider::Rotary);
    sustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 0, 0);
    sustainSlider.setRange(0.0, 1.0);   // LEVEL (IMPORTANT)
    sustainSlider.setValue(0.8);        // default audible sustain
    addAndMakeVisible(sustainSlider);

    // attack slider
    attackSlider.setSliderStyle(juce::Slider::Rotary);
    attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    attackSlider.setRange(0.001, 2.0);   // LEVEL (IMPORTANT)
    attackSlider.setValue(0.05);       // default audible attack
    addAndMakeVisible(attackSlider);

    // decay  Slider
    decaySlider.setSliderStyle(juce::Slider::Rotary);
    decaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    decaySlider.setRange(0.001, 2.0);   // LEVEL (IMPORTANT)
    decaySlider.setValue(0.2);       // default audible decay
    addAndMakeVisible(decaySlider);

    // release Slider
    releaseSlider.setSliderStyle(juce::Slider::Rotary);
    releaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    releaseSlider.setRange(0.01, 3.0); // LEVEL (IMPORTANT)
    releaseSlider.setValue(0.5);       // default audible release
    addAndMakeVisible(releaseSlider);

    addAndMakeVisible(keyboardComponent);

    // ===== Accent colour for all knobs =====
    auto accent = juce::Colour(0xff5ddcff);

    cutoffSlider.setColour(juce::Slider::rotarySliderFillColourId, accent);
    vibratoSlider.setColour(juce::Slider::rotarySliderFillColourId, accent);
    breathSlider.setColour(juce::Slider::rotarySliderFillColourId, accent);

    attackSlider.setColour(juce::Slider::rotarySliderFillColourId, accent);
    decaySlider.setColour(juce::Slider::rotarySliderFillColourId, accent);
    sustainSlider.setColour(juce::Slider::rotarySliderFillColourId, accent);
    releaseSlider.setColour(juce::Slider::rotarySliderFillColourId, accent);


    auto setupSlider = [this](juce::Slider& s)
        {
            s.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
            s.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
            addAndMakeVisible(s);
        };

    setupSlider(attackSlider);
    setupSlider(decaySlider);
    setupSlider(sustainSlider);
    setupSlider(releaseSlider);
    setupSlider(cutoffSlider);
    setupSlider(vibratoSlider);

    startTimerHz(30);




    
    addAndMakeVisible(recordStartButton);
    addAndMakeVisible(recordStopButton);
    addAndMakeVisible(playMidiButton);

   

   

    // ===== metronome beat selector =====

    addAndMakeVisible(beatSelector);

    beatSelector.addItem("4/4", 1);
    beatSelector.addItem("3/4", 2);
    beatSelector.addItem("6/8", 3);
    beatSelector.addItem("7/8", 4);

    beatSelector.setSelectedId(1);

    beatSelector.onChange = [this]()
        {
            if (beatSelector.getSelectedId() == 1) beatsPerBar = 4;
            else if (beatSelector.getSelectedId() == 2) beatsPerBar = 3;
            else if (beatSelector.getSelectedId() == 3) beatsPerBar = 6;
        };

    // ===== Preset Buttons Setup =====
    savePresetButton.setButtonText("Save Preset");
    loadPresetButton.setButtonText("Load Preset");

    addAndMakeVisible(savePresetButton);
    addAndMakeVisible(loadPresetButton);

    sampleNameLabel.setColour(juce::Label::backgroundColourId,
        juce::Colour(0xff1e293b));


    // ===== Save Preset Logic =====
    savePresetButton.onClick = [this]()
        {
            juce::FileChooser chooser(
                "Save Preset",
                juce::File::getSpecialLocation(
                    juce::File::userDocumentsDirectory),
                "*.xml");

            chooser.launchAsync(
                juce::FileBrowserComponent::saveMode
                | juce::FileBrowserComponent::canSelectFiles,
                [this](const juce::FileChooser& fc)
                {
                    auto file = fc.getResult();

                    if (file != juce::File())
                    {
                        juce::ValueTree preset("WaveFlutePreset");

                        preset.setProperty("attack", attackSlider.getValue(), nullptr);
                        preset.setProperty("decay", decaySlider.getValue(), nullptr);
                        preset.setProperty("sustain", sustainSlider.getValue(), nullptr);
                        preset.setProperty("release", releaseSlider.getValue(), nullptr);
                        preset.setProperty("cutoff", cutoffSlider.getValue(), nullptr);
                        preset.setProperty("vibrato", vibratoSlider.getValue(), nullptr);
                        preset.setProperty("breath", breathSlider.getValue(), nullptr);

                        if (auto stream = file.createOutputStream())
                        {
                            preset.writeToStream(*stream);
                        }
                    }
                });
        };



    loadPresetButton.onClick = [this]()
        {
            juce::PopupMenu menu;

            menu.addItem(1, "Flute");
            menu.addItem(2, "Piano");
            menu.addItem(3, "Violin");
            menu.addItem(4, "panflute");
            menu.addItem(5, "flute2");
            menu.addItem(6, "guitar");
            menu.addItem(7, "flute3");
          


            menu.showMenuAsync(juce::PopupMenu::Options(),
                [this](int result)
                {
                   
                    if (result == 1)
                    {
                        auto file = juce::File::getSpecialLocation(
                            juce::File::userDesktopDirectory
                        ).getChildFile("flute.wav");

                        loadSample(file);
                    }
                    else if (result == 2)
                    {
                        auto file = juce::File::getSpecialLocation(
                            juce::File::userDesktopDirectory
                        ).getChildFile("piano.wav");

                        loadSample(file);
                    }
                    else if (result == 3)
                    {
                        auto file = juce::File::getSpecialLocation(
                            juce::File::userDesktopDirectory
                        ).getChildFile("violin.wav");

                        loadSample(file);
                    }
                    else if (result == 4)
                    {
                        auto file = juce::File::getSpecialLocation(
                            juce::File::userDesktopDirectory
                        ).getChildFile("panflute.wav");

                        loadSample(file);
                    }
                    else if (result == 5)
                    {
                        auto file = juce::File::getSpecialLocation(
                            juce::File::userDesktopDirectory
                        ).getChildFile("flute2.wav");

                        loadSample(file);
                    }
                    else if (result == 6)
                    {
                        auto file = juce::File::getSpecialLocation(
                            juce::File::userDesktopDirectory
                        ).getChildFile("guitar.wav");

                        loadSample(file);
                    }

                    else if (result == 7)
                    {
                        auto file = juce::File::getSpecialLocation(
                            juce::File::userDesktopDirectory
                        ).getChildFile("flute3.wav");

                        loadSample(file);
                    }
                });
        };
    addAndMakeVisible(sampleNameLabel);

    sampleNameLabel.setText("No Sample Loaded", juce::dontSendNotification);
    sampleNameLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    sampleNameLabel.setJustificationType(juce::Justification::centred);


    addAndMakeVisible(audioSettingsButton);

    audioSettingsButton.onClick = [this]()
        {
            showAudioPanel = !showAudioPanel;
            deviceSelector.setVisible(showAudioPanel);
        };





    // Label setup helper
    auto setupLabel = [this](juce::Label& l, const juce::String& text)
        {
            l.setText(text, juce::dontSendNotification);
            l.setJustificationType(juce::Justification::centred);
            l.setColour(juce::Label::textColourId, juce::Colours::white);
            addAndMakeVisible(deviceSelector);
        };

    setupLabel(attackLabel, "ATTACK");
    setupLabel(decayLabel, "DECAY");
    setupLabel(sustainLabel, "SUSTAIN");
    setupLabel(releaseLabel, "RELEASE");
    setupLabel(cutoffLabel, "CUTOFF");
    setupLabel(vibratoLabel, "VIBRATO");


    addAndMakeVisible(metronomeButton);

    bpmSlider.setRange(40, 200);
    bpmSlider.setValue(120);
    addAndMakeVisible(bpmSlider);

    // ===== AUTO LOAD LAST SAMPLE =====
    juce::File presetFile = juce::File::getSpecialLocation(
        juce::File::userDocumentsDirectory
    ).getChildFile("preset.txt");

    if (presetFile.existsAsFile())
    {
        juce::String path = presetFile.loadFileAsString();
        loadSample(juce::File(path));
    }



}





MainComponent::~MainComponent()
{
    shutdownAudio();
}


// ==================================================
void MainComponent::prepareToPlay(int samplesPerBlock, double sampleRate)
{
    smoothCutoff.reset(sampleRate, 0.05);   // 50 ms smooth
    smoothVibrato.reset(sampleRate, 0.05);
    smoothBreath.reset(sampleRate, 0.05);


    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 1;


    currentSampleRate = sampleRate;


    // 👉 REVERB use same spec
    spec.numChannels = 2;   // stereo for reverb
    reverb.prepare(spec);

    osc.prepare(spec);
    osc.setFrequency(440.0f);

    lfo.prepare(spec);
    lfo.setFrequency(5.0f); // vibrato speed

    lowPass.reset();
    lowPass.prepare(spec);
    lowPass.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    lowPass.setCutoffFrequency(cutoffSlider.getValue());

    adsrParams.attack = attackSlider.getValue();
    adsrParams.decay = decaySlider.getValue();
    adsrParams.sustain = sustainSlider.getValue();
    adsrParams.release = releaseSlider.getValue();
    adsr.setParameters(adsrParams);

    formatManager.registerBasicFormats();

    sampler.clearVoices();
    sampler.clearSounds();



    reverb.prepare(spec);


    // Add voices
    for (int i = 0; i < 8; ++i)
        sampler.addVoice(new juce::SamplerVoice());










    if (reader.get() != nullptr)
    {
        juce::BigInteger allNotes;
        allNotes.setRange(0, 128, true);

        sampler.clearSounds();
        sampler.clearVoices();

        sampler.addVoice(new juce::SamplerVoice());

        sampler.addSound(
            new juce::SamplerSound(
                "Flute",
                *reader,          // ✅ SAFE HERE
                allNotes,
                60,               // root MIDI note
                0.01,             // attack
                0.1,              // release
                10.0              // max length
            )
        );
    }




    sampler.setCurrentPlaybackSampleRate(sampleRate);





}


void MainComponent::loadSample(juce::File file)
{
    reader.reset(formatManager.createReaderFor(file));
    sampleNameLabel.setText("Loaded: " + file.getFileName(),
        juce::dontSendNotification);

    if (reader.get() != nullptr)
    {
        juce::BigInteger allNotes;
        allNotes.setRange(0, 128, true);

        sampler.clearVoices();
        sampler.clearSounds();

        sampler.addVoice(new juce::SamplerVoice());

        sampler.addSound(
            new juce::SamplerSound(
                file.getFileName(),
                *reader,
                allNotes,
                60,
                0.01,
                0.1,
                10.0
            )
        );
    }
}

// ==================================================
void MainComponent::getNextAudioBlock(
    const juce::AudioSourceChannelInfo& bufferToFill)
{
    smoothCutoff.setTargetValue(cutoffSlider.getValue());
    smoothVibrato.setTargetValue(vibratoSlider.getValue());
    smoothBreath.setTargetValue(breathSlider.getValue());


    bufferToFill.clearActiveBufferRegion();

    // MIDI from on-screen keyboard
    juce::MidiBuffer midi;
    keyboardState.processNextMidiBuffer(
        midi,
        bufferToFill.startSample,
        bufferToFill.numSamples,
        true
    );

    // SAMPLER renders audio
    sampler.renderNextBlock(
        *bufferToFill.buffer,
        midi,
        bufferToFill.startSample,
        bufferToFill.numSamples
    );

    if (metronomeButton.getToggleState())
    {
        double bpm = bpmSlider.getValue();
        int samplesPerBeat = (int)(currentSampleRate * 60.0 / bpm);

        auto* left = bufferToFill.buffer->getWritePointer(0);
        auto* right = bufferToFill.buffer->getNumChannels() > 1
            ? bufferToFill.buffer->getWritePointer(1)
            : nullptr;

        for (int i = 0; i < bufferToFill.numSamples; ++i)
        {
            if (samplesUntilNextTick <= 0)
            {
                float click = (currentBeat == 0) ? 1.0f : 0.5f; // strong / weak beat

                left[i] += click;
                if (right) right[i] += click;

                samplesUntilNextTick = samplesPerBeat;

                currentBeat++;
                if (currentBeat >= beatsPerBar)
                    currentBeat = 0;
            }

            samplesUntilNextTick--;
        }
    }


    float vibrato = smoothVibrato.getNextValue();
    float breath = smoothBreath.getNextValue();

    auto* leftData = bufferToFill.buffer->getReadPointer(0);
    auto* rightData = bufferToFill.buffer->getNumChannels() > 1
        ? bufferToFill.buffer->getReadPointer(1)
        : nullptr;

    float leftPeak = 0.0f;
    float rightPeak = 0.0f;

    for (int i = 0; i < bufferToFill.numSamples; ++i)
    {
        leftPeak = juce::jmax(leftPeak, std::abs(leftData[i]));

        if (rightData != nullptr)
            rightPeak = juce::jmax(rightPeak, std::abs(rightData[i]));
    }

    leftLevel = leftPeak;
    rightLevel = rightPeak;

    juce::dsp::AudioBlock<float> block(*bufferToFill.buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);

    reverb.process(context);
}






// ==================================================
void MainComponent::releaseResources() {}

// ==================================================
void MainComponent::paint(juce::Graphics& g)
{


    g.setColour(juce::Colours::white);
    g.setFont(24.0f);

    g.drawText("Wave Flute Fusion",
        0, 5, getWidth(), 40,
        juce::Justification::centred);

    g.setColour(juce::Colour(0xff2a2a2a));

    // ADSR box
    g.drawRoundedRectangle(20, 60, getWidth() - 40, 140, 12.0f, 2.0f);

    // EXPRESSION box
    g.drawRoundedRectangle(20, 190, getWidth() - 40, 140, 12.0f, 2.0f);

   


    // Background
    g.fillAll(juce::Colour(0xff1e1e1e));

    // Title
    g.setColour(juce::Colours::white);
    g.setFont(24.0f);
    g.drawText("Wave Flute Fusion",
        getLocalBounds().removeFromTop(40),
        juce::Justification::centred);

    // Section boxes
    g.setColour(juce::Colour(0xff2e2e2e));

    // ADSR section
    g.drawRoundedRectangle(20, 60, 960, 150, 12.0f, 2.0f);

    // Expression section
    g.drawRoundedRectangle(20, 230, 960, 150, 12.0f, 2.0f);

    // Keyboard section
    g.drawRoundedRectangle(20, 400, 960, 120, 12.0f, 2.0f);

    // Section titles
    g.setFont(14.0f);
    g.drawText("AMPLITUDE (ADSR)", 30, 65, 300, 20, juce::Justification::left);
    g.drawText("EXPRESSION & TONE", 30, 235, 300, 20, juce::Justification::left);

    // ===== Output Meter =====
    g.setColour(juce::Colours::darkgrey);
    g.fillRect(920, 420, 15, 80); // L background
    g.fillRect(945, 420, 15, 80); // R background

    g.setColour(juce::Colours::limegreen);

    int meterHeightL = (int)(leftLevel * 80.0f);
    int meterHeightR = (int)(rightLevel * 80.0f);

    g.fillRect(920, 420 + (80 - meterHeightL), 15, meterHeightL);
    g.fillRect(945, 420 + (80 - meterHeightR), 15, meterHeightR);

}


// ==================================================

void MainComponent::setMainUIVisible(bool visible)
{
    attackSlider.setVisible(visible);
    decaySlider.setVisible(visible);
    sustainSlider.setVisible(visible);
    releaseSlider.setVisible(visible);

    cutoffSlider.setVisible(visible);
    vibratoSlider.setVisible(visible);
    breathSlider.setVisible(visible);

    keyboardComponent.setVisible(visible);
}

void MainComponent::resized()
{
    sampleNameLabel.setBounds(getWidth() / 2 - 150, 45, 300, 20);

  

    int margin = 20;
    int width = getWidth();

    // ================= TOP BAR =================
    int topY = 10;
    int buttonW = 110;
    int gap = 10;

    audioSettingsButton.setBounds(margin, topY, 140, 30);
    metronomeButton.setBounds(160, topY, 120, 30);
    bpmSlider.setBounds(290, topY, 80, 30);
    beatSelector.setBounds(width - 360, 10, 80, 30);

    savePresetButton.setBounds(width - 260, topY, 120, 30);
    loadPresetButton.setBounds(width - 130, topY, 120, 30);
   
    // ================= RECORDING MODE =================
  

    // ================= ADSR =================
    int adsrY = 70;
    int knobSize = 130;
    int spacing = (width - 5 * knobSize) / 5;

    attackSlider.setBounds(spacing, adsrY, knobSize, knobSize);
    decaySlider.setBounds(spacing * 2 + knobSize, adsrY, knobSize, knobSize);
    sustainSlider.setBounds(spacing * 3 + knobSize * 2, adsrY, knobSize, knobSize);
    releaseSlider.setBounds(spacing * 4 + knobSize * 3, adsrY, knobSize, knobSize);

    // labels
    attackLabel.setBounds(attackSlider.getX(), adsrY + knobSize, knobSize, 20);
    decayLabel.setBounds(decaySlider.getX(), adsrY + knobSize, knobSize, 20);
    sustainLabel.setBounds(sustainSlider.getX(), adsrY + knobSize, knobSize, 20);
    releaseLabel.setBounds(releaseSlider.getX(), adsrY + knobSize, knobSize, 20);

    // ================= EXPRESSION =================
    int exprY = 250;
    int spacing2 = (width - 3 * knobSize) / 4;

    cutoffSlider.setBounds(spacing2, exprY, knobSize, knobSize);
    vibratoSlider.setBounds(spacing2 * 2 + knobSize, exprY, knobSize, knobSize);
    breathSlider.setBounds(spacing2 * 3 + knobSize * 2, exprY, knobSize, knobSize);

    cutoffLabel.setBounds(cutoffSlider.getX(), exprY + knobSize, knobSize, 20);
    vibratoLabel.setBounds(vibratoSlider.getX(), exprY + knobSize, knobSize, 20);
    breathLabel.setBounds(breathSlider.getX(), exprY + knobSize, knobSize, 20);

    // ================= KEYBOARD =================
    keyboardComponent.setBounds(40, getHeight() - 120, getWidth() - 80, 80);

    // ================= AUDIO PANEL =================
    deviceSelector.setBounds(getWidth() - 300, 50, 280, 300);
}




// ==================================================
void MainComponent::handleIncomingMidiMessage(
    juce::MidiInput*, const juce::MidiMessage& message)
{
    keyboardState.processNextMidiEvent(message);

   
}
// UI module update