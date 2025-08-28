#include "PluginProcessor.h"
#include "PluginEditor.h"

GraphicEQ15BandAudioProcessorEditor::GraphicEQ15BandAudioProcessorEditor(GraphicEQ15BandAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    setSize(1000, 300); // Increased height for better label visibility
    
    for (int i = 0; i < 15; ++i)
    {
        sliders[i].setSliderStyle(juce::Slider::LinearVertical);
        sliders[i].setRange(-12.0, 12.0, 0.1);
        sliders[i].setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
        sliders[i].setTextValueSuffix(" dB");
        addAndMakeVisible(sliders[i]);
        
        sliderAttachments[i] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            processor.parameters, "GAIN_" + juce::String(i), sliders[i]);
    }
}

GraphicEQ15BandAudioProcessorEditor::~GraphicEQ15BandAudioProcessorEditor()
{
}

void GraphicEQ15BandAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    
    g.setColour(juce::Colours::white);
    g.setFont(14.0f); // Slightly smaller font for better fit
    for (int i = 0; i < 15; ++i)
    {
        g.drawText(juce::String(processor.frequencies[i]) + " Hz",
                   sliders[i].getBounds().withHeight(30).translated(0, -40), // Increased offset for label
                   juce::Justification::centred);
    }
}

void GraphicEQ15BandAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(10).withTrimmedTop(40).withTrimmedBottom(20); // Add top/bottom padding
    int sliderWidth = area.getWidth() / 15;
    
    for (int i = 0; i < 15; ++i)
    {
        sliders[i].setBounds(area.removeFromLeft(sliderWidth));
    }
}