namespace audio_plugin {
namespace {
class RandomPointGenerator {
public:
  RandomPointGenerator(juce::Rectangle<float> bounds)
      : bounds_{std::move(bounds)} {}

  juce::Point<float> generate() {
    return {random_.nextFloat() * bounds_.getWidth(),
            random_.nextFloat() * bounds_.getHeight()};
  }

private:
  juce::Rectangle<float> bounds_;
  juce::Random random_{42};
};

void drawNoise(juce::Graphics& g,
               const juce::Rectangle<float>& localBounds,
               float density) {
  constexpr auto radius = 1.f;
  const auto componentArea = localBounds.getWidth() * localBounds.getHeight();
  constexpr auto pointArea = juce::MathConstants<float>::pi * radius * radius;
  const auto pointsToPaint =
      static_cast<int>(density * componentArea / pointArea);

  RandomPointGenerator pointGenerator{localBounds};
  for ([[maybe_unused]] const auto i : std::views::iota(0, pointsToPaint)) {
    const auto point = pointGenerator.generate();
    g.fillEllipse(point.x, point.y, radius, radius);
  }
}

auto getAudiowideTypeface() {
  static const auto result = juce::Typeface::createSystemTypefaceFor(
      assets::AudiowideRegular_ttf, assets::AudiowideRegular_ttfSize);
  return juce::FontOptions{result};
}

auto getBungeeShadeTypeface() {
  static const auto result = juce::Typeface::createSystemTypefaceFor(
      assets::BungeeShadeRegular_ttf, assets::BungeeShadeRegular_ttfSize);
  return juce::FontOptions{result};
}

enum class Colors : size_t { darkGray, lightGray, orange };
juce::Colour getColor(Colors colorName) {
  static const std::array colors{juce::Colour{0xFF4E4E4E},
                                 juce::Colour{0xFFF5F5F5},
                                 juce::Colour{0xFFEF7600}};
  return colors.at(juce::toUnderlyingType(colorName));
}
}  // namespace

void Background::paint(juce::Graphics& g) {
  g.fillAll(getColor(Colors::lightGray));

  g.setColour(juce::Colours::black);
  g.setOpacity(0.15f);
  drawNoise(g, getLocalBounds().toFloat(), 0.68f);
}

RotarySlider::RotarySlider() {
  setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
  setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
}

void RotarySlider::paint(juce::Graphics& g) {
  // knob
  auto knobBounds = getLocalBounds().reduced(10).toFloat();
  g.setColour(getColor(Colors::darkGray));
  g.fillEllipse(knobBounds);
  knobBounds.reduce(2.f, 2.f);
  g.setColour(getColor(Colors::lightGray));
  g.fillEllipse(knobBounds);
}

PluginEditor::PluginEditor(PluginProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p) {
  juce::ignoreUnused(processorRef);

  addAndMakeVisible(background_);

  flangerLabel_.setText("FLANGER", juce::dontSendNotification);
  flangerLabel_.setBorderSize({0, 0, 0, 0});
  flangerLabel_.setMinimumHorizontalScale(1.f);
  flangerLabel_.setFont(
      getBungeeShadeTypeface().withPointHeight(40.f).withKerningFactor(.76f));
  flangerLabel_.setJustificationType(
      juce::Justification::horizontallyJustified | juce::Justification::top);
  flangerLabel_.setColour(juce::Label::textColourId,
                          getColor(Colors::darkGray));
  addAndMakeVisible(flangerLabel_);

  modRateLabel_.setText("mod rate", juce::dontSendNotification);
  modRateLabel_.setFont(getAudiowideTypeface().withPointHeight(20.f));
  modRateLabel_.setJustificationType(juce::Justification::centredTop);
  modRateLabel_.setColour(juce::Label::textColourId,
                          getColor(Colors::darkGray));
  addAndMakeVisible(modRateLabel_);

  addAndMakeVisible(modRateSlider_);

  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(520, 250);
}

void PluginEditor::resized() {
  // This is generally where you'll want to lay out the positions of any
  // subcomponents in your editor.
  background_.setBounds(getLocalBounds());
  auto flangerLabelBounds = getLocalBounds();
  flangerLabelBounds.removeFromLeft(27);
  flangerLabelBounds.removeFromTop(13);
  // add extra space so that the letters aren't squished
  flangerLabelBounds.setWidth(flangerLabelBounds.getWidth() + 50);
  flangerLabel_.setBounds(flangerLabelBounds);

  modRateLabel_.setBounds(181, 187, 158, 29);

  modRateSlider_.setBounds(210, 95, 100, 100);
}
}  // namespace audio_plugin
