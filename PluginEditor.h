#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include <JuceHeader.h>
#include "PluginProcessor.h"

class GraphicEQ15BandAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    GraphicEQ15BandAudioProcessorEditor(GraphicEQ15BandAudioProcessor&);
    ~GraphicEQ15BandAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    GraphicEQ15BandAudioProcessor& processor;
    std::array<juce::Slider, 15> sliders;
    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>, 15> sliderAttachments;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GraphicEQ15BandAudioProcessorEditor)
};

#endif // PLUGINEDITOR_H_INCLUDED