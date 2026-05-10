# Task 0

> [!WARNING]
> Don't test audio using headphones. If there's a glitch, it may impair your hearing.

Your first task is to compile the code on the `main` branch. Do this **BEFORE** the workshop starts. Although that may seem simple, it typically is the biggest hurdle students face.

1. Install the necessary tools:
   1. git
   2. CMake 3.25 or newer
   3. Latest version of the C++ compiler
      1. GCC on Linux
      2. AppleClang on macOS (ships with Xcode)
      3. MSVC on Windows (ships with Visual Studio)
   4. Latest version of the C++ build system
      1. make on Linux
      2. Xcode build system on macOS (ships with Xcode)
      3. MSBuild on Widnows (ships with Visual Studio)
      4. Ninja
   5. IDE of your choice, e.g., CLion, Visual Studio Code, Visual Studio, Xcode.
      1. ⚠️ On Windows and macOS, you still need to install Visual Studio and Xcode respectively, even if you decide to use a different IDE, e.g., Visual Studio Code.
   6. To run the workshop plugin, you need a digital audio workstation (DAW). I recommend [Reaper](https://www.reaper.fm/).
1. (Windows-only) If you use CMake from the command line and you get "missing C++ compiler" information or other weird errors, you need to put the compiler on your PATH. For this, you need to run a batch script. Assuming, you have Visual Studio 2022 installed, you can run the following command to initialize C++ environment in your shell.

    ```bash
    "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
    ```

1. (Linux-only) Before generating the project, you need to install GCC 16 (if you don't already have it installed) and [JUCE dependencies](https://github.com/juce-framework/JUCE/blob/master/docs/Linux%20Dependencies.md). You can do it with the following commands:
    ```bash
    sudo apt update
    sudo apt install gcc-16 g++-16
    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-16 100
    sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-16 100
    sudo update-alternatives --set gcc /usr/bin/gcc-16
    sudo update-alternatives --set g++ /usr/bin/g++-16
    sudo apt install libasound2-dev libjack-jackd2-dev \
        ladspa-sdk \
        libcurl4-openssl-dev  \
        libfreetype-dev libfontconfig1-dev \
        libx11-dev libxcomposite-dev libxcursor-dev libxext-dev libxinerama-dev libxrandr-dev libxrender-dev \
        libwebkit2gtk-4.1-dev \
        libglu1-mesa-dev mesa-common-dev
    ```
1. To compile the project, you can use the CMake command-line interface.
    ```bash
    # Generate the buildsystem
    cmake --preset default
    # Check CMakePresets.json for other generators

    # Build the project
    cmake --build --preset default
    ```

    If you're using an IDE, follow their instructions on working with CMake-based projects. I am using CLion from JetBrains, because it integrates great with JUCE modules, but you don't have to. Visual Studio Code, Visual Studio, or Xcode work great.
1. Run the compiled projects. The built targets are in platform- and buildsystem-specific folders. For the `default` preset and the `make` buildsystem, run
    ```bash
    build/Debug/music_player/music_player # you should hear a guitar sound with the flanger effect applied
    build/Debug/test/AudioPluginTest # runs tests: has file data/Guitar_5th_FlangerEnd2EndTestOutput.wav been created?
    build/Debug/AudioPlugin_artefacts/Debug/Standalone/CppOnline2026WorkshopFlanger.app/Contents/MacOS/CppOnline2026WorkshopFlanger # start the GUI app
    ```
1. Watch [the workshop preview on YouTube](https://youtu.be/VSr5o7kR1KM?si=L30yOa4LO5kV6HpT). It conveys basic knowledge that we'll use in the workshop. Watching it before the workshop will greatly aid you in writing code.

## Bonus tip

The complete solution to all the tasks are present on the `main` branch. If at any time during the workshop you need to check the reference solution, compare the difference between your working tree and the `main` branch.

```bash
git diff main
```

You can also compare against the branch of the next task.

In Visual Studio Code, there's the wonderful GitLens plugin that allows you to visually compare against another branch via the "GitLens: Compare Working Tree with..." command.
