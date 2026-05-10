# Tasks

## Part 1: Building a music player in C++

In this part of the workshop, you will learn the basics of operating on audio samples and interacting with operating system to play them back.

### Task: Play back sine

```bash
git checkout task/generate-sine
```

Your first task is to play back a test tone through speakers: a 220 Hz sine. You will accomplish this using the PortAudio library, which will show you the low-level audio playback through your operating system's audio driver API. For now, just focus on the *music_player/source/main.cpp* file.

1. Add `pa_ex::Initializer` and `pa_ex::Stream` members to `MusicPlayer`.
1. Initialize the stream in the constructor with the following configuration:
    1. No input channels
    1. 2 output channels
    1. 32-bit float-based processing
    1. No desired frame buffer size
    1. 44.1 kHz sampling rate
    1. `audioCallback()` as the audio callback
1. Start the stream in the `start()` function.
1. Stop the stream in the `stop()` function.
1. Iterate over frames in `processBlock()` and for each frame
    1. Generate the sine sample, using the discrete sine formula: $\sin(\phi)$, where $\phi$ is the phase, which is incremented by $2\pi f / f_s$ with each frame ($f$ is the frequency in Hz and $f_s$ is the sampling rate in Hz).
    1. Copy the sine sample to all channels
    1. Increment the phase
1. Compile the `music_player` project and exectue it. Can you hear the sine? Remember not to use headphones for testing!
1. Extract sine generation to a `SineGenerator` class with the following declaration:

```cpp
class SineGenerator {
public:
  void setFrequency(wolfsound::Frequency f);
  void prepareToPlay(double sampleRate);
  void processBlock(AudioBuffer buffer);
};
```

### Task: Play back audio file

```bash
git checkout task/file-player
```

Now, we may turn our attention to the _fx/include/fx/fx.h_ file. Here you will implement a class that the `MusicPlayer` can use to retrieve samples from a file.

1. Implement the `fx::FilePlayer` class capable of playing back audio files.
    1. `#include` the _AudioFile.h_ header.
    1. Make `AudioFile file_` a member of `FilePlayer`.
    1. `load()` the file in the constructor.
    1. Play back samples in the `processBlock()` member function.
        1. Play back all channels from the file or the playback (whichever is smaller).
        1. Fill the played back channels not present in the file with silence.
        1. When the file stops playing back, output silence on all channels.
1. Replace `SineGenerator` with `FilePlayer` in `MusicPlayer`.
1. Pass the path to _data/Guitar_5th.wav_ file to its constructor.
1. Test that the file is played back when running `music_player`.
1. Allow an arbitrary processing chain in `MusicPlayer`.
    1. Introduce an `AudioProcessor` interface with two virtual functions: `prepareToPlay(double sampleRate, int maxFramesPerBuffer, int channelCount)` and `processBlock(AudioBuffer)`. Make `SineGenerator` and `FilePlayer` implement this interface.
    1. Pass a `std::vector<std::unique_ptr<AudioProcessor>>` to `MusicPlayer` instead of hardcoding the actual class that does the processing. Store the `vector` as a member.
    1. Call `prepareToPlay()` of all processors in the constructor.
    1. Call `processBlock()` of all processors in the `audioCallback()` function.

### Task: Flanger audio effect

```bash
git checkout task/flanger-effect
```

1. Add a `SineGenerator lfo_` member to the `Flanger` class.
1. "Prepare" it in `Flanger::prepareToPlay()`.
1. In `Flanger::processBlock()`, generate the LFO signal to the LFO buffer so that it can be used by all channels.
1. Calculate `middleDelay_` and `maxDelay_` in samples in `Flanger::ChannelProcessor::prepareToPlay()` assuming that the maximum delay in seconds is 0.002.
1. Implement the flanger difference equation in `Flanger::ChannelProcessor::processBlock()`: 

    1. Calculate the modulated-delay value

        $$m = s_\text{LFO,unipolar}[n]D.$$

    2. Calculate the helper signal sample

        $$x_h[n] = x[n] + \text{feedback } x_h[n-D/2].$$

    3. Calculate the output sample

        $$y[n] = \text{blend }x_h[n] + \text{feedforward } x_h[n - m].$$
1. Emplace an `fx::Flanger` instance in the `vector` passed to `MusicPlayer`, just after the `FilePlayer` (so that flanger impacts the played back file).
1. Run the `music_player` app; is the flanger effect applied?

## Part 2: Audio plugin in JUCE C++ framework

From now on, we will work exclusively in the *audio_plugin* folder (apart from the root _CMakeLists.txt_ file). Our plugin consists of two classes: `PluginProcessor` for audio processing and plugin-related duties and `PluginEditor` for displaying the UI.

### Task: Flanger plugin

```bash
git checkout task/flanger-plugin
```

1. Add an `fx::Flanger flanger_` member to `PluginProcessor`.
1. Call `flanger_.prepareToPlay()` in `PluginProcessor::prepareToPlay`.
1. Apply the flanger effect to the audio in the `PluginProcessor::processBlock()` function. `interleave()` and `deinterleave()` functions have been provided for you, since JUCE does not use interleaved buffers.
1. Build the AudioPlugin_VST3 project. You should see "installation" messages in the compiler output.
1. Open the DAW of your choice (e.g., Reaper) and see if you can add the plugin on a track.
1. Test the effect by placing _data/Guitar_5th.wav_ on a track and adding the plugin onto it.

### Task: Parameters

```bash
git checkout task/add-parameter
```

To control an audio plugin, we need plugin parameters. There are a few approaches to defining your plugin parameters. In the workshop, we will use a utility class from my library `wolfsound-dsp-utils`.

1. Add a `juce::AudioParameterFloat& lfoFrequency` member to `PluginProcessor::Parameters` struct.
1. Define it by calling `builder.add<juce::AudioParameterFloat>()` in `Parameters`'s constructor and passing appropriate arguments. I suggest an LFO in the [0.01, 10] range with steps of size 0.1 Hz. Consider adding a custom "Hz" label. As the default, pass the initial value of `Flanger::Parameters::lfoFrequency`.
1. Remember to call `flanger_.setParameters()` with the current parameter value in `PluginProcessor::processBlock()`.
1. Check that you can see the parameter value changing (and being remembered) in the generic editor and the UI-less editor.
1. Draw an automation curve for this parameter. Does it work as intended?

## Part 3: Plugin GUI in JUCE C++ framework

### Task: Create a custom editor

```bash
git checkout task/create-custom-editor
```

1. Return `true` from `PluginProcessor::hasEditor()`.
1. Return an owning `PluginEditor` instance from `PluginProcessor::createEditor()`.
1. Set the size of the editor in its constructor according to the Figma design.

### Task: Add a slider controlling the modulation rate

```bash
git checkout task/add-slider
```

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

```bash
git checkout task/style-components
```

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

```bash
git checkout task/add-labels
```

As you can see, our UI design in Figma uses 3 labels: one for the "FLANGER" logo, one for the modulation rate knob, and one for the knob's value. In this task, you will add the first two.

1. Add `juce::Label valueLabel_` and `juce::Label modRateLabel_` members to `PluginEditor`.
1. Call `addAndMakeVisible()` in `PluginEditor`'s constructor.
1. Call `setBounds()` in `PluginEditor::resized()`.
1. Call `setText()`, `setJustificationType()`, and `setColour()` on the labels in `PluginEditor`'s constructor. Don't worry about font, font size, and exact placement yet.

### Task: Add value label

```bash
git checkout task/add-value-label
```

As you can see in the Figma design, we have a label indicating slider's value. The value label should update on every slider change to reflect the current value. In JUCE, the standard way to achieve this is by observing the slider value. 

1. Add another `juce::Label` instance, this time as a member of the `RateSlider` class.
1. Remember to call `addAndMakeVisible()` on it in the constructor and `setBounds()` in the overridden `resized()` method.
1. Set its color and justification.
1. Make it transparent to clicks by calling `setInterceptsMouseClicks()` with appropriate arguments on it.
1. Make `RateSlider` inherit from `juce::Slider::Listener` and override its `sliderValueChanged()` method to update the value label on each slider value change. You can use the `getTextFromValue()` method to retrieve the value as a string.
6. Observe the slider (=itself) in `RateSlider`'s constructor using `addListener()`. Remember to remove itself from the observer list by calling `removeListener()` in the destructor.

### Task: Fonts

```bash
git checkout task/add-custom-fonts
```

So far our labels use the stock Font that ships with JUCE. That's fine for hobby projects, but professional-grade audio plugins always use custom fonts in their GUIs. Since we don't know whether the chosen font is present on user's machine, we must bundle the fonts with the plugin binary. As you can see, we have two fonts in the Figma projects and both are preset as TTF files in the _audio_plugin/assets/_ folder. You task is to add them to the project and use them in the GUI.

1. Find where we define plugin's binary data in _CMakeLists.txt_. Add both TTF files as binary data. Regenerate and recompile the project.
1. Read the font files from binary data. For, this, use the `juce::createSystemTypefaceFor()` static function with appropriate arguments. You have to do it only once per plugin instantiation and only if the user opens the GUI.
1. Note that the `juce::Label` class has a member function named `setFont()`. Call this function on label objects with appropriate `juce::FontOptions` objects. Note that `FontOptions` allows you to configure the font size.

## Homework

```bash
git checkout homework/draw-background-noise
```

Based on the Figma design file, fill the background with programmatically generated "noise." For this, look up Figma noise parameters and implement the `drawNoise()` function in PluginProcessor.cpp accordingly. Good luck!

