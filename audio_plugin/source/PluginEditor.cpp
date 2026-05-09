namespace audio_plugin {
namespace {
void drawNoise(juce::Graphics& g,
               const juce::Rectangle<float>& localBounds,
               float density) {
  constexpr auto radius = 1.f;
  const auto componentArea = localBounds.getWidth() * localBounds.getHeight();
  constexpr auto pointArea = juce::MathConstants<float>::pi * radius * radius;
  const auto pointsToPaint =
      static_cast<int>(density * componentArea / pointArea);

  // TODO: Generate and paint random points
  for ([[maybe_unused]] const auto i : std::views::iota(0, pointsToPaint)) {
    juce::ignoreUnused(g);
  }
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
  setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
  constexpr auto angle = juce::degreesToRadians(20.f + 90.f);
  setRotaryParameters(juce::Slider::RotaryParameters{
      .startAngleRadians = juce::MathConstants<float>::twoPi - angle,
      .endAngleRadians = juce::MathConstants<float>::twoPi + angle,
      .stopAtEnd = true,
  });

  // TODO: Add value label and observe the slider for value changes to update it
  // TODO: Set valueLabel_'s font
}

juce::String RotarySlider::getTextFromValue(double v) {
  return {v, 2};
}

void RotarySlider::paint(juce::Graphics& g) {
  const auto bounds = getLocalBounds().toFloat();
  const auto rotaryParams = getRotaryParameters();
  const auto proportionOfValue = valueToProportionOfLength(getValue());
  const auto valueAngle =
      static_cast<float>(rotaryParams.startAngleRadians +
                         proportionOfValue * (rotaryParams.endAngleRadians -
                                              rotaryParams.startAngleRadians));

  // canal
  constexpr auto canalWidth = 6.f;
  static const auto arcStyle =
      juce::PathStrokeType{canalWidth, juce::PathStrokeType::JointStyle::curved,
                           juce::PathStrokeType::EndCapStyle::butt};
  const auto canalBounds = bounds.reduced(canalWidth / 2.f);
  juce::Path canal;
  canal.addCentredArc(
      canalBounds.getCentreX(), canalBounds.getCentreY(),
      canalBounds.getWidth() / 2.f, canalBounds.getHeight() / 2.f, 0.f,
      rotaryParams.startAngleRadians, rotaryParams.endAngleRadians, true);
  g.setColour(getColor(Colors::darkGray));
  g.strokePath(canal, arcStyle);

  // value arc
  juce::Path valueArc;
  valueArc.addCentredArc(canalBounds.getCentreX(), canalBounds.getCentreY(),
                         canalBounds.getWidth() / 2.f,
                         canalBounds.getHeight() / 2.f, 0.f,
                         rotaryParams.startAngleRadians, valueAngle, true);
  g.setColour(getColor(Colors::orange));
  g.strokePath(valueArc, arcStyle);

  // knob
  auto knobBounds = bounds.reduced(10);
  g.setColour(getColor(Colors::darkGray));
  g.fillEllipse(knobBounds);
  constexpr auto borderThickness = 2.f;
  g.setColour(getColor(Colors::lightGray));
  g.fillEllipse(knobBounds.reduced(borderThickness));

  // value indicator
  const auto radius = knobBounds.getWidth() / 2.f;
  const auto radiusLine = juce::Line<float>::fromStartAndAngle(
      knobBounds.getCentre(), radius, valueAngle);
  constexpr auto valueIndicatorLength = 11.66f;
  const auto valueIndicator =
      radiusLine.withShortenedStart(radius - valueIndicatorLength);
  g.setColour(getColor(Colors::darkGray));
  g.drawLine(valueIndicator, borderThickness);
}

PluginEditor::PluginEditor(PluginProcessor& p)
    : AudioProcessorEditor(&p),
      processorRef(p),
      sliderAttachment_{p.getParameterRefs().lfoFrequency, modRateSlider_} {
  juce::ignoreUnused(processorRef);

  addAndMakeVisible(background_);

  // TODO: Add "FLANGER" label
  // TODO: Set "FLANGER" label font

  // TODO: Add "mod rate [hz]" label
  // TODO: Set "mod rate [hz]" font

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

  // TODO: Set labels' bounds

  modRateSlider_.setBounds(210, 95, 100, 100);
}
}  // namespace audio_plugin
