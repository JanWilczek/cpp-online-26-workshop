---
marp: true
theme: default
---

# Beginner’s Guide to C++ Audio Programming with the JUCE Framework

---

<!-- paginate: true -->
<!-- footer: "&copy; WolfSound Jan Wilczek 2026" --->

<style>
h1 {
  color: #EF7600;
}

.inline-images {
    display: flex;
    height: 80%; /* control location on y-axis */
    justify-content: space-evenly;
    align-items: center;
}

img[alt~="align-right"] {
    float: right;
}

img[alt~="align-left"] {
    float: left;
}
</style>

# Introduction

![w:300 align-right](img/0002-round.png)

* Jan Wilczek [Yan Vil-check]
* Audio programming consultant & coach
* Founder of TheWolfSound.com blog & YouTube channel
* WolfTalk podcast host
* Online course creator
  * DSP Pro on digital audio signal processing
  * Official JUCE C++ framework audio plugin development course

<!-- I want to enable everyone with Internet access to learn audio programming: the science and art of processing sound using code -->
<!-- Now it's your turn to introduce yourselves! Say a few sentences about yourself and what do you want to achieve through this workshop. -->

---

# Rules

<style scoped>section{font-size:2em;}</style>

* Workshop repo: https://github.com/JanWilczek/cpp-online-26-workshop
* These slides: _docs/slides/cpponline-2026-workshop-slides.md_
* Task descriptions: _docs/tasks/tasks.md_
* Please, have your camera turned on, if possible
  * If you need a break, or some time in quiet, just turn off your camera; we'll know you're not there
* We won't split into break rooms, as there's only 7 of us
* Always ask, when in doubt: feel free to interrupt me
* It's ok not to understand the entire code
* I am here to help you and depending on the needs, I will want to help everyone
* I may skip some of the tasks if we're short on time

---

# Prerequisite: Task 0

_docs/tasks/task0.md_

<!-- Who has done task 0 and compiled the project that's present on the `main` branch? -->
<!-- Who has watched my workshop preview? -->

---

# Part 1: Building a music player in C++

---

# Task: Play back a sine

```bash
git checkout task/generate-sine
```

Focus: *music_player/source/main.cpp*

We'll be using the PortAudio library by Ross Bencina, Phil Burk, et al.

---

# Task: Play back a sine

## Initializing PortAudio

```cpp
const auto error = Pa_Initialize();
//...
if (error == paNoError) {
  Pa_Terminate();
}
```

---

# Task: Play back a sine

## Initializing PortAudio

```cpp
class MusicPlayer {
  //...
  pa_ex::Initializer initializer_;
};
```

---

# Task: Play back a sine

## Creating a stream

```cpp
PaStream* stream;
const auto error = Pa_OpenDefaultStream( &stream, /* many arguments */);
//...
if (stream != nullptr && error == paNoError) {
  Pa_CloseStream(stream_);
}
```

---

# Task: Play back a sine

## Creating a stream

```cpp
class MusicPlayer {
explicit MusicPlayer()
      : stream_{/* many arguments */} {}
  //...
  pa_ex::Stream stream_;
};
```

---

# Task: Play back a sine

## Generating the sine

$$s[n] = A\sin(2\pi f n / f_s),$$

where

* $n$ is the unitless sample index
* $s[n]$ is the discrete output signal
* $f$ is the frequency in Hz
* $f_s$ is the sample rate
* $A$ is the unitless amplitude
* $\pi = 3.14159\dots$

---

# Task: Play back a sine

## Generating the sine

$$s[n] = A\sin(2\pi f n / f_s),$$

```cpp
auto phase_ = 0.f;
//...
constexpr auto amplitude = 0.25f;
const auto outputSample = amplitude * std::sin(phase_);

// output the sample...

constexpr auto frequency = 220.f;
phase_ += 2 * std::numbers::pi_v<float> * frequency / sampleRate_;
```

<!-- Now, all should start working on the task -->
<!-- A word of comment: you can do sine generation and sound playback easily with JUCE -->

---

# Task: Play back an audio file

```bash
git checkout task/file-player
```

Focus:

* _fx/include/fx/fx.h_
* *music_player/source/main.cpp*

We'll be using the AudioFile library by AdamStark.

---

# Task: Flanger audio effect

```bash
git checkout task/flanger-effect
```

Focus:

* _fx/include/fx/fx.h_
* *music_player/source/main.cpp*

---

# Task: Flanger audio effect

## What is Flanger?

Input:

<audio src="../data/Guitar_5th.wav" controls/>

Output:

<audio src="../data/guitar_5th_FlangerTest_FileEnd2EndOutput.wav" controls/>

---

# Task: Flanger audio effect

## What is flanger?

A DSP algorithm that can be depicted as:

* DSP diagram
* Difference equations
* Textual description
* Literature reference
  * ”We will implement equations X-Y from paper Z”

<!-- Difference equations are easiest to implement in code. Diagrams are great for analysis or visual programming languages -->

---

# Sources of DSP algorithms

* Books
* Research papers
* Online blogs & videos
* Online forums
* External consultancy
* Self-design (e.g., through experiments)

---

# How to read DSP diagrams?

* TODO:
* A few simple examples
  * identity
  * delay by 1
  * delay by N
  * nonlinearity

---

# Task: Flanger audio effect

## Flanger DSP diagram

![w:800](img/WorkshopFlangerAnnotated.png)

<style scoped>section{font-size:1.5em;}</style>

Source: J. Dattoro. _Effect design, part 2: Delay-line modulation and chorus._ J. Audio Eng. Soc., 45(10): 764–788, October 1997.

<!-- Let's write out the difference equations by tracing back the output to the input -->

---

# Task: Flanger audio effect

## Flanger difference equations

1. Calculate the modulated-delay value

    $$m = s_\text{LFO,unipolar}[n]D.$$

2. Calculate the helper signal sample

    $$x_h[n] = x[n] + \text{feedback } x_h[n-D/2].$$

3. Calculate the output sample

    $$y[n] = \text{blend }x_h[n] + \text{feedforward } x_h[n - m].$$

<!-- Now it's time to implement it -> look at the code to show the structure (esp. `ChannelProcessor`) -->

---

# Part 2: Audio plugin in JUCE C++ framework

<!-- Have you worked with audio plugins and DAWs before? -->

---

# Digital audio workstation (DAW)

![w:850](img/AbletonLive.png)

---

# DAW plugins

![h:500](img/Many%20plugin%20hosts%20with%20many%20plugins.png)

---

# DAW plugins

![](img/PluginAPI.png)

---

# Popular plugin APIs

* Audio Unit v3 (AUv3) by Apple for macOS and iOS
* **Virtual Studio Technology 3 (VST3) by Steinberg**
* Avid Audio eXtensions (AAX) by Avid
* LV2 for Linux
* CLever Audio Plug-in (CLAP)

---

# Many plugin formats = development nightmare

![w:650](img/Plugins%20in%20formats.png)

---

# Plugin format API abstraction → plugin frameworks

![w:900](img/JUCE%20is%20the%20magical%20tool.png)

---

# Plugin lifecycle

![w:530](img/processor_lifecycle.png)

---

# Task: Flanger plugin

```bash
git checkout task/flanger-plugin
```

<!-- Now it's time to code! -->

---

# Task: Parameters

```bash
git checkout task/add-parameter
```

---

# Purpose of plugin parameters
 
1. DSP control
1. Generic UI
1. Visualization

---

# JUCE parameter classes hierarchy

<img src="img/AudioProcessorParameterClassHierarchy.svg" width="100%">

---

# JUCE parameter classes
 
<style scoped>section{font-size:2em;}</style>

* `AudioParameterBool`
  * `true`/`false`
* `AudioParameterInt`
  * integer in a closed range
  * e.g., "1" from {0, 1, 2}
* `AudioParameterFloat`
  * real value from a closed range
  * e.g., "0.25" from [0, 2]
* `AudioParameterChoice`
  * a value from a fixed set of named options
  * e.g., "lowpass" from {"lowpass", "highpass"}

---

# Purpose of JUCE parameter classes

* Single source of truth
* Plugin version management
* Plugin presets
* GUI thread-audio thread synchronization (explained in more detail later)

---

# JUCE parameter class

## Creation (at plugin construction)

* Instantiate dynamically, e.g., using `std::make_unique<>()`
* Retrieve a reference or a pointer
* Call `PluginProcessor::addParameter()`

## Usage

* Retrieve the current parameter value in `PluginProcessor::processBlock()`

---

# JUCE parameter class

<style scoped>section{font-size:1.5em;}</style>

We'll use my `wolfsound::JuceParameterHolder` utility.

```cpp
struct Parameters {
  juce::AudioParameterFloat& floatParam;
};

class PluginProcessor {
  //...
  Parameters parameters_;
  wolfsound::JuceParameterHolder parameterHolder_;
};

// .cpp file
PluginProcessor::PluginProcessor(
    JuceParameterHolder::Builder builder)
    : parameters_.floatParam{builder.add<juce::AudioParameterFloat>(
          "floatParam",
          "Float Param",
          juce::NormalisableRange{1.f, 10.f},
          5.f)},
      parameterHolder_{std::move(builder).build(*this)} {}
```

<!-- Now is the time for implementation -->

---

# Part 3: Plugin GUI in JUCE C++ framework

---

# Processor-editor split

![](img/Processor-Editor%20split.png)

---

# Editor lifecycle

![height:480px](img/plugin_lifecycle.png)

---

# Task: Create a custom editor

```bash
git checkout task/create-custom-editor
```

---

# Task: Add a slider controlling the modulation rate

```bash
git checkout task/add-slider
```

![w:800px](img/FlangerUI.png)

---

# JUCE component system

1. Everything you see on the screen is a juce::Component subclass instance
1. Each component has 0, 1, or more child components ➡️ component hierarchy

---

# JUCE coordinate system

![width:600](img/JUCE%20UI%20coordinate%20system.png)

---

# Adding a new child component

```cpp
class PluginEditor : ... {
  struct BackgroundComponent : juce::Component {

  };
  BackgroundComponent background;

public:
  PluginEditor(...) {
    addAndMakeVisible(background);
  }

  void resized() override {
    background.setBounds(getLocalBounds());
  }
};
```

---

# Task: Style components

```bash
git checkout task/style-components
```

---

# Task: Add labels

```bash
git checkout task/add-labels
```

---

# Task: Add value label

```bash
git checkout task/add-value-label
```

---

# Task: Fonts

```bash
git checkout task/add-custom-fonts
```

---

# JUCE binary data

```cmake
juce_add_binary_data(
    audio_plugin_assets
  NAMESPACE
    audio_plugin::assets
  SOURCES
    audio_plugin/assets/MyFont.ttf
    audio_plugin/assets/SomeImage.png
)
```

---

# JUCE binary data

```cpp
namespace audio_plugin::assets {
    extern const char*   MyFont_ttf;
    const int            MyFont_ttfSize = 47676;

    extern const char*   SomeImage_png;
    const int            SomeImage_ttfSize = 300764;

    //... some utilities
}
```

---

# JUCE binary data

```cpp
auto getMyFont() {
  static const auto result = juce::Typeface::createSystemTypefaceFor(
      assets::MyFont_ttf, assets::MyFont_ttfSize);
  return juce::FontOptions{result};
}
//...
label.setFont(getMyFont());
```

---

# Homework

```bash
git checkout homework/draw-background-noise
```

---

# Thank you!

## Where to go from here

* Official JUCE course (free): wolfsoundacademy.com/juce
* DSP course: wolfsoundacademy.com/dsp
  * **40% discount code: CPPO26 (valid until May 26)**
* WolfSound blog: thewolfsound.com
* WolfSound YouTube channel: youtube.com/@WolfSoundAudio
* contact@thewolfsound.com

