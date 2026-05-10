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
* **These slides: _docs/slides/cpponline-2026-workshop-slides.md_**
* **Task descriptions: _docs/tasks/tasks.md_**

<!-- Now it's your turn to introduce yourselves! -->

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

---

# Part 2: Audio plugin in JUCE C++ framework

---

# Task: Flanger plugin

---

# Task: Parameters

---

# Part 3: Plugin GUI in JUCE C++ framework

---

# Task: Create a custom editor

---

# Task: Add a slider controlling the modulation rate

---

# Task: Style components

---

# Task: Add labels

---

# Task: Add value label

---

# Task: Fonts

---

# Homework
