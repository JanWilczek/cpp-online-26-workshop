# Tasks

## Task 1: Play back sine

## Task 2: Play back audio file

## Task 3: Flanger audio effect

- research & design (in short explained)
- difference equation
- implementation
- interface considerations
- unification with sine generator
- reuse of sine generator

## Part -2: Audio plugin in JUCE C++ framework

From now on, we will work exclusively in the *audio_plugin* folder (apart from the _CMakeLists.txt_ file).

### Task

1. Add an `fx::Flanger flanger_` member to `PluginProcessor`.
1. Call `flanger_.prepareToPlay()` in `PluginProcessor::prepareToPlay`.
1. Apply the flanger effect to the audio in the `PluginProcessor::processBlock()` function. `interleave()` and `deinterleave()` functions have been provided for you, since JUCE does not use interleaved buffers.

### Task: Parameters

To control an audio plugin, we need plugin parameters. There are a few approaches to defining your plugin parameters. In the workshop, we will use a utility class from my library `wolfsound-dsp-utils`.

1. Add a `juce::AudioParameterFloat& lfoFrequency` member to `PluginProcessor::Parameters` struct.
1. Define it by calling `builder.add<juce::AudioParameterFloat>()` in `Parameters`'s constructor and passing appropriate arguments. I suggest an LFO in the [0.01, 10] range with steps of size 0.1 Hz. Consider adding a custom "Hz" label. As the default, pass the initial value of `Flanger::Parameters::lfoFrequency`.
1. Remember to call `flanger_.setParameters()` with the current parameter value in `PluginProcessor::processBlock()`.
1. Check that you can see the parameter value changing (and being remembered) in the generic editor and the UI-less editor.
1. Draw an automation curve for this parameter. Does it work as intended?

## Part -1: Plugin GUI in JUCE C++ framework

### Task: Create a custom editor

1. Return `true` from `PluginProcessor::hasEditor()`.
1. Return an owning `PluginEditor` instance from `PluginProcessor::createEditor()`.
1. Set the size of the editor in its constructor according to the Figma design.

### Task: Add a slider controling the modulation rate

1. Add a `juce::Slider modRateSlider_` member to `PluginEditor`.
1. In `PluginEditor`'s constructor,
    1. Call `addAndMakeVisible(modRateSlider_)`,
    1. `setSliderStyle()` to "rotary vertical drag",
    1. `setTextBoxStyle()` to "no text box".
1. In `PluginEditor::resized()` method, set the slider's bounds according to the Figma design.
1. Connect the slider to the parameter.
    1. Add a `juce::SliderParameterAttachment` member in `PluginEditor`
    1. In `PluginEditor`'s constructor, initialize it with the reference to the `lfoFrequency` parameter (that you can obtain from the processor) and the slider reference
1. Compile and test. Does dragging the slider change the parameter?

### Task: Style components

There are 3 ways to customize (style) JUCE components:

1. call their member functions,
1. subclass them and override their `paint()` method, or
1. subclass `juce::LookAndFeel_V4` and override the methods relevant for components you want to style.

In this workshop, we will use the first two. In this task, we will add a custom background and customize our rate slider.

#### Adding background

1. Create a new class called `Background` that inherits from `juce::Component`. The class should override only the `paint()` method of the `Component` class.
1. In the overridden `paint()` method, call `g.fillAll(getColor(Colors::lightGray)`. `getColor()` is a helper function to avoid using hex values directly in code.
1. Add `Background` as a `PluginEditor` member called `background_`.
3. Call `addAndMakeVisible(background_)` in `PluginEditor`'s constructor.
1. Call `background_.setBounds()` in `PluginEditor::resized()` method. Pass in the result of `getLocalBounds()` call because we want the `background_` to fill the entire editor area.
1. Compile and check your result.

#### Customizing the slider

1. Create a subclass of the `juce::Slider` class called `RateSlider`.
1. Change `modRateSlider_` type to `RateSlider`.
1. Override its `paint()` method.
1. In the `paint()` method, draw the slider according to its current value.
    1. Copy the code from `juce::LookAndFeel_V4::drawRotarySlider()` and adapt it to the fact that `paint()` is a member of `RateSlider`.
    1. Tweak the drawing code according to Figma design. It's OK not to accomplish everything here, as this can get pretty lengthy.
1. As an additional challenge, use `setRotaryParameters()` in a custom `RotarySlider`'s constructor to change the extreme angles of the knob.

### Task: Add labels

As you can see, our UI design in Figma uses 3 labels: one for the "FLANGER" logo, one for the modulation rate knob, and one for the knob's value. In this task, you will add the first two.

1. Add `juce::Label valueLabel_` and `juce::Label modRateLabel_` members to `PluginEditor`.
1. Call `addAndMakeVisible()` in `PluginEditor`'s constructor.
1. Call `setBounds()` in `PluginEditor::resized()`.
1. Call `setText()`, `setJustificationType()`, and `setColour()` on the labels in `PluginEditor`'s constructor. Don't worry about font, font size, and exact placement yet.

### Task: Add value label

As you can see in the Figma design, we have a label indicating slider's value. The value label should update on every slider change to reflect the current value. In JUCE, the standard way to achieve this is by observing the slider value. 

1. Add another `juce::Label` instance, this time as a member of the `RateSlider` class.
1. Remember to call `addAndMakeVisible()` on it in the constructor and `setBounds()` in the overridden `resized()` method.
1. Set its color and justification.
1. Make it transparent to clicks by calling `setInterceptsMouseClicks()` with appropriate arguments on it.
1. Make `RateSlider` inherit from `juce::Slider::Listener` and override its `sliderValueChanged()` method to update the value label on each slider value change. You can use the `getTextFromValue()` method to retrieve the value as a string.
6. Observe the slider (=itself) in `RateSlider`'s constructor using `addListener()`. Remember to remove itself from the observer list by calling `removeListener()` in the destructor.

### Task: Fonts

So far our labels use the stock Font that ships with JUCE. That's fine for hobby projects, but professional-grade audio plugins always use custom fonts in their GUIs. Since we don't know whether the chosen font is present on user's machine, we must bundle the fonts with the plugin binary. As you can see, we have two fonts in the Figma projects and both are preset as TTF files in the _audio_plugin/assets/_ folder. You task is to add them to the project and use them in the GUI.

1. Find where we define plugin's binary data in _CMakeLists.txt_. Add both TTF files as binary data. Regenerate and recompile the project.
1. Read the font files from binary data. For, this, use the `juce::createSystemTypefaceFor()` static function with appropriate arguments. You have to do it only once per plugin instantiation and only if the user opens the GUI.
1. Note that the `juce::Label` class has a member function named `setFont()`. Call this function on label objects with appropriate `juce::FontOptions` objects. Note that `FontOptions` allows you to configure the font size.

### Homework

Based on the Figma design file, fill the background with programmatically generated "noise." For this, look up Figma noise parameters and implement the `drawNoise()` function in PluginProcessor.cpp accordingly. Good luck!

