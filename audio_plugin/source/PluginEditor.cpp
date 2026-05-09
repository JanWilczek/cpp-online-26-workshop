namespace audio_plugin {
namespace {
enum class Colors : size_t { darkGray, lightGray, orange };
[[maybe_unused]] juce::Colour getColor(Colors colorName) {
  static const std::array colors{juce::Colour{0xFF4E4E4E},
                                 juce::Colour{0xFFF5F5F5},
                                 juce::Colour{0xFFEF7600}};
  return colors.at(juce::toUnderlyingType(colorName));
}
}  // namespace
PluginEditor::PluginEditor(PluginProcessor& p)
    : AudioProcessorEditor(&p),
      processorRef(p),
      sliderAttachment_{p.getParameterRefs().lfoFrequency, modRateSlider_} {
  juce::ignoreUnused(processorRef);

  // TODO: Add background

  // TODO: Add "FLANGER" label
  // TODO: Set "FLANGER" label font

  // TODO: Add "mod rate [hz]" label
  // TODO: Set "mod rate [hz]" font

  modRateSlider_.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
  modRateSlider_.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox,
                                 true, 0, 0);
  addAndMakeVisible(modRateSlider_);

  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(520, 250);
}

void PluginEditor::resized() {
  // This is generally where you'll want to lay out the positions of any
  // subcomponents in your editor.
  // TODO: Set background's size

  // TODO: Set labels' bounds

  modRateSlider_.setBounds(210, 95, 100, 100);
}
}  // namespace audio_plugin
