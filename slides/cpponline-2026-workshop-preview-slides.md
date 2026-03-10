---
marp: true
theme: default
# class: invert
---

![bg](img/PreviewTitleCard.png)

---

<!-- paginate: true -->
<!-- footer: "&copy; WolfSound Jan Wilczek 2026 (TheWolfSound.com)" --->

# Assumptions

1. You know basic C++ and you are able to write at least a small C++-oriented program
1. You know what CMake is and how to invoke it (no need to know how to exactly write CMakeLists.txt files)
1. You are interested in music, e.g., you play a musical instrument, you like listening to music, or you perform electronic music


---

# Sound in modern software

1. Streaming/videoconferencing just like we are now
2. Video games
3. Tools for musicians and music producers
4. Embedded applications in microphones, speakers, and smartphones
5. automatic speech recognition
6. text-to-speech

---

# We want to play back sound; how to do it in C++?


---

# What is sound anyway?

---

# What is sound anyway?

![Acoustic wave in the air](img/acoustc_wave.png)

---

# How does sound enter the computer?

![](img/wave_to_voltage.png)

---

# How does sound enter the computer?

![](img/voltage_to_ram.png)

---

# Samples

![](img/SampledVoltage.png)

---

# Sampling period & sampling rate

![](img/SamplingPeriod.png)

---

# How does sound exit the computer?

![](img/ram_to_voltage.png)

---

# How does sound exit the computer?

![](img/voltage_to_wave.png)

---

# ADC/DAC

![](img/AudioInterface1.png)
![](img/AudioInterface2.png)

---

# How to play back sound using C++?

---

# How to play back sound using C++?

## No `std::audio`!

---

# How to play back sound using C++?

## No `std::audio`!

We need to use OS-specific APIs, for example,

* CoreAudio on macOS
* DirectSound on Windows
* ALSA on Linux

---

# Isn’t there a cross-platform library that can do it for us?

---

# Isn’t there a cross-platform library that can do it for us?

* PortAudio
* JUCE
* other

---

# How to play back sound using PortAudio?

```cpp
#include <portaudio.h>

const auto error = Pa_Initialize();
```

---

# How to play back sound using PortAudio?

```cpp
const auto error = Pa_Initialize();
//...
if (error == paNoError) {
  Pa_Terminate();
}
```

---

# How to play back sound using PortAudio?

```cpp
class Initializer {
public:
  Initializer() : _error{Pa_Initialize()} {}

  ~Initializer() {
    if (_error == paNoError) {
      Pa_Terminate();
    }
  }

private:
  PaError _error;
};
```

---

# How to play back sound using PortAudio?

```cpp
constexpr auto sampleRate = 44100.;
PaStream* stream;
const auto error = Pa_OpenDefaultStream(
                &stream, 0, 2, paFloat32, sampleRate,
                static_cast<unsigned long>(paFramesPerBufferUnspecified),
                [](const void* input,
                   void* output,
                   unsigned long frameCount,
                   const PaStreamCallbackTimeInfo* timeInfo,
                   PaStreamCallbackFlags statusFlags,
                   void* userData) {
                  return paContinue;
                },
                nullptr);
```

---

# How to play back sound using PortAudio?

```cpp
PaError Pa_OpenDefaultStream( PaStream** stream,
                              int numInputChannels,
                              int numOutputChannels,
                              PaSampleFormat sampleFormat,
                              double sampleRate,
                              unsigned long framesPerBuffer,
                              PaStreamCallback *streamCallback,
                              void *userData );
```

---

# Channels

![](img/Stereo.png)

---

# Sample format

* `float` or `double` in the [-1, 1] range (`paFloat32`)
* `int`-like (not used in apps or plugins)

---

# Frames per buffer

![](img/Frame.png)

---

# Samples vs frames

* Buffer size = frame count  = samples per channel
* 480 stereo frames in a buffer -> 960 samples

---

# Audio callback

```cpp
typedef int PaStreamCallback(
    const void *input,
    void *output,
    unsigned long frameCount,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData );
```

* Actual audio processing
* Called ~100 times per second (depending on the sample rate and the buffer size)
* Called on a real-time, high-priority thread
* Must complete within a deadline (soft real-time constraint)

---

# Let's generate a sine

---

# Sine formula

$$s(t) = A\sin(2\pi f t),$$

where
* $t$ is time in seconds
* $s(t)$ is the output signal (a function of time)
* $f$ is the frequency in Hz
* $A$ is the unitless amplitude
* $\pi = 3.14159\dots$

---

# Sine formula

$$s[n] = A\sin(2\pi f n / f_s),$$

where
* $n$ is the unitless sample index
* $s[n]$ is the discrete output signal
* $f$ is the frequency in Hz
* $f_s$ is the sample rate
* $A$ is the unitless amplitude
* $\pi = 3.14159\dots$

---

# Sine formula

$$s[n] = A\sin(2\pi f n / f_s),$$

```cpp
constexpr auto amplitude = 0.25f;
const auto outputSample = amplitude * std::sin(_phase);

// output the sample...

constexpr auto frequency = 220.f;
_phase += 2 * std::numbers::pi_v<float> * frequency / _sampleRate;
```

---

# What is `output`?

```cpp
typedef int PaStreamCallback(
    const void *input,
    void *output,
    unsigned long frameCount,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData );
```

TODO: Add line highlighting and highlight the line with output

---

# Audio buffer

![](img/Buffer.png)

---

# Audio buffer

![](img/AudioBuffer.png)

---

# Interleaved vs non-interleaved samples

## Non-interleaved

![](img/AudioBuffer.png)

## Interleaved

![](img/AudioBufferInterleaved.png)

---

# Accessing interleaved samples as non-interleaved

![](img/AudioBufferInterleaved.png)

```cpp
using AudioBuffer =
    std::mdspan<float, std::dextents<int, 2>, std::layout_left>;

auto buffer = AudioBuffer{static_cast<float*>(output), 2, 480};
```

![](img/AudioBuffer.png)

---

# Output the sine sample to all channels

```cpp
for (const auto frame : std::views::iota(0, buffer.extent(1))) {
  constexpr auto amplitude = 0.25f;
  const auto outputSample = amplitude * std::sin(_phase);

  for (const auto channel : std::views::iota(0, buffer.extent(0))) {
    buffer[channel, frame] = outputSample;
  }

  constexpr auto frequency = 220.f;
  _phase += 2 * std::numbers::pi_v<float> * frequency / _sampleRate;
}
```

---

# Don’t test using headphones!

---

# Cleanup

```cpp
Pa_StopStream(stream);
Pa_CloseStream(stream);
```

---

```cpp
class Stream {
public:
  template <typename... Ts>
  explicit Stream(Ts&&... args)
      : _error{Pa_OpenDefaultStream(&_stream, std::forward<Ts>(args)...)} {}

  ~Stream() {
    if (_stream != nullptr && _error == paNoError) {
      Pa_CloseStream(_stream);
    }
  }

  // deleted copy constructor & assignment operator
  // move constructor & assignment operator

  void start() { Pa_StartStream(_stream); }

  void stop() { Pa_StopStream(_stream); }

private:
  PaStream* _stream{nullptr};
  PaError _error{paNoError};
};
```

---

# Summary (zoom-out) of what we needed to provide to start audio playback



---

    1. Connect to the audio device (if exists)

---

    2. Register the audio callback

---

    3. Start playing at a set sample rate

---

    4. Stop playback

---

    5. Close the connection to the audio device

---

7. Ok, we know how to play back a sound. How to play back an audio file?

---

    1. Use the AudioFile library

---

8.  Now, we can create a command-line music player!

---

9.  Why C++ for audio?

---

10. We have a nice music player, but what if we want to boost the bass?

---

    1. An audio effect

---

    2. You need to know DSP

---

    3. The processing is applied in the audio callback and needs to maintain state between the calls

---

    4. What if we want to have more effects? → audio processors

---

    5. What if we want to have more audio files playing back in parallel? → tracks

---

    6. What if we want to record audio on those tracks? → DAW

---

    7. How to add effects to a DAW? → plugins

---

    8. Plugin formats

---

    9. Plugin format API abstraction → plugin frameworks

---

    10. JUCE

---

11. Realtime programming in the audio callback; a brief introduction (how audio programming differs from regular programming)

---

12. Now we have recorded a guitar and we want to apply a flanger effect onto it; how to go about it? → DSP research, block diagrams, and difference equations

---

13. Show the completed plugin GUI and how it sounds

---

14. Why using AI to learn audio programming is a bad idea

---

15. Attend the workshop to learn more

---

16. Summary
