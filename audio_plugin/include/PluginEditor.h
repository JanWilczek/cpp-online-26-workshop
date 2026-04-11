#pragma once

namespace audio_plugin {
class Background : public juce::Component {
public:
  void paint(juce::Graphics& g) override;
};

class RotarySlider : public juce::Slider {
public:
  RotarySlider();
  void paint(juce::Graphics& g) override;
};

class PluginEditor : public juce::AudioProcessorEditor {
public:
  explicit PluginEditor(PluginProcessor&);

  void resized() override;

private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  PluginProcessor& processorRef;
  Background background_;
  juce::Label flangerLabel_;
  juce::Label modLabel_;
  RotarySlider modRateSlider_;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};
}  // namespace audio_plugin
