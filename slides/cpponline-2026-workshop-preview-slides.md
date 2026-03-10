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

4. Ok, we know how sound gets in and gets out; how to play back sound using C++?

---

    1. Nothing in the C++ standard

---

    2. We need to use OS-specific APIs (give examples but don’t go into too much detail here)

---

    3. Isn’t there a cross-platform library that can do it for us?

---

    4. PortAudio, JUCE, others

---

5. How does playing back a sound look in PortAudio?

---

    1. Show the code

---

    2. Channels

---

    3. Sample format

---

    4. Frames per buffer

---

        1. Samples vs frames

---

    5. Audio callback

---

        1. Generate a sine

---

        2. Samples are interleaved

---

        3. Sine formula

---

        4. Don’t test using headphones!

---

6. Summary (zoom-out) of what we needed to provide to start audio playback

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
