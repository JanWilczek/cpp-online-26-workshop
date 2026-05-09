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

### Task

- add Flanger to PluginProcessor
- add prepareToPlay()
- add reset()
- add processBlock() between interleave/deinterleave

### Task: Parameters

- define LFO frequency parameter using builder.add<>
- setParameters()

## Part -1: Plugin GUI in JUCE C++ framework

### Task

- show UI-less editor
- create editor
- change its size

### Task

- add slider
- connect slider to parameter

### Task: Styling

- add white background
- customize slider's look

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

