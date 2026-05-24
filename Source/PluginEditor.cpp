#include "PluginProcessor.h"
#include "PluginEditor.h"

PinkGuitarFXAudioProcessorEditor::PinkGuitarFXAudioProcessorEditor (PinkGuitarFXAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setLookAndFeel(&pinkLook);
    addAndMakeVisible(tuner);

    juce::StringArray ids = {"gate", "gain", "wide", "low", "high"};
    juce::StringArray names = {"GATE", "DRIVE", "WIDE", "BASS", "TREB"};

    for (int i = 0; i < ids.size(); ++i) {
        auto* s = knobs.add(new juce::Slider());
        s->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        s->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 15);
        addAndMakeVisible(s);
        sAttaches.add(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvts, ids[i], *s));
        
        auto* l = labels.add(new juce::Label());
        l->setText(names[i], juce::dontSendNotification);
        l->setJustificationType(juce::Justification::centred);
        addAndMakeVisible(l);
    }

    masterSlider.setSliderStyle(juce::Slider::LinearVertical);
    addAndMakeVisible(masterSlider);
    sAttaches.add(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.apvts, "master", masterSlider));

    addAndMakeVisible(cabBox);
    cabBox.addItemList(audioProcessor.apvts.getParameter("cab")->getAllValueStrings(), 1);
    cAttach = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "cab", cabBox);

    startTimerHz(60);
    setSize (800, 600);
}

PinkGuitarFXAudioProcessorEditor::~PinkGuitarFXAudioProcessorEditor() { setLookAndFeel(nullptr); }

void PinkGuitarFXAudioProcessorEditor::timerCallback() { repaint(); }

void PinkGuitarFXAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xFF0D0D0F));
    float level = audioProcessor.rmsLevelLeft.load();
    
    g.setColour(juce::Colours::hotpink.withAlpha(0.2f));
    g.setFont(juce::FontOptions(28.0f).withStyle("Bold"));
    g.drawText("safoniy 2012", 20, 20, 200, 40, juce::Justification::left);
    
    // Спектрограмма (упрощенная для стабильности)
    g.setColour(juce::Colours::hotpink.withAlpha(0.1f));
    for (int i = 0; i < getWidth(); i += 20) {
        float h = (std::abs(std::sin(i * 0.1f)) + 0.5f) * 30.0f * level * 6.0f;
        g.fillRect((float)i, (float)getHeight() - h - 60, 12.0f, h);
    }
}

void PinkGuitarFXAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(20);
    tuner.setBounds(area.removeFromTop(100).reduced(10));
    masterSlider.setBounds(area.removeFromRight(60).removeFromTop(350));
    cabBox.setBounds(area.removeFromBottom(40).removeFromRight(150));

    int w = area.getWidth() / 5;
    for (int i = 0; i < knobs.size(); ++i) {
        auto cell = area.removeFromLeft(w).reduced(10);
        labels[i]->setBounds(cell.removeFromTop(20));
        knobs[i]->setBounds(cell);
    }
}
