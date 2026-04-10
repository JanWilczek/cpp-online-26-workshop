namespace audio_plugin {
namespace {
juce::Point<float> getRandomPoint(const juce::Rectangle<float>& bounds) {
  static auto random = juce::Random{42};
  return {random.nextFloat() * bounds.getWidth(),
          random.nextFloat() * bounds.getHeight()};
}
}  // namespace

void Background::paint(juce::Graphics& g) {
  g.fillAll(juce::Colour{0xFFF5F5F5});

  g.setColour(juce::Colours::black);
  g.setOpacity(0.15f);
  // density 68%
  // size 1.4
  constexpr auto density = 0.68f;
  constexpr auto radius = 1.f;
  const auto componentArea = static_cast<float>(getWidth() * getHeight());
  constexpr auto pointArea = juce::MathConstants<float>::pi * radius * radius;
  const auto pointsToPaint =
      static_cast<int>(density * componentArea / pointArea);

  for ([[maybe_unused]] const auto i : std::views::iota(0, pointsToPaint)) {
    const auto point = getRandomPoint(getLocalBounds().toFloat());
    g.fillEllipse(point.x, point.y, radius, radius);
  }
}

PluginEditor::PluginEditor(PluginProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p) {
  juce::ignoreUnused(processorRef);

  addAndMakeVisible(background_);

  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(520, 250);
}

void PluginEditor::resized() {
  // This is generally where you'll want to lay out the positions of any
  // subcomponents in your editor..
  background_.setBounds(getLocalBounds());
}
}  // namespace audio_plugin
