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
```

---

# Task: Play back a sine

## Creating a stream

```cpp

```

---

# Task: Play back an audio file

---

# Task: Flanger audio effect

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
