# 🐺 WolfSound's Audio Plugin Template

![Cmake workflow success badge](https://github.com/JanWilczek/audio-plugin-template/actions/workflows/cmake.yml/badge.svg)

[**✨FREE JUCE AUDIO PLUGIN DEVELOPMENT COURSE✨**](https://wolfsoundacademy.com/juce?utm_source=github-audio-plugin-template&utm_medium=referral)

> [!IMPORTANT]
> This is version 2 of the audio-plugin-template, which contains important improvements and updates over version 1. Don't be surprised if the setup doesn't match 1:1 what I have shown in the 2023 YouTube video 😉 Using v2 is even simpler than v1.

* Want to create an audio plugin (e.g., a VST3 plugin) with C++ but don't know how to go about?
* Heard about the [JUCE C++ framework](https://github.com/juce-framework/JUCE) but not sure how to start a JUCE project?
* Want to use CMake with JUCE but don't know how?
* Want to be able to easily integrate third-party C++ libraries into your project?
* Want to unit test your audio plugin?
* Want to ensure the maximum safety of your software?
* And all this with a click of a button?

Well, this template lets you start your JUCE C++ audio plugin project right away with a CMake-based project structure. It involves

* a clear repo structure
* C++ 23 standard
* effortless handling of third-party dependencies with the CPM package manager; use the C++ libraries you want together with JUCE
* highest warning level and "treat warnings as errors"
* ready-to-go unit test project with GoogleTest
* integrates with Visual Studio, Visual Studio Code, Xcode, CLion, and NeoVim

Additionally

* continuous integration made easy with GitHub Actions: build and run tests on the main branch and on every pull request
* automatic clang-format of C++ files run on every commit; don't worry about code formatting anymore!

I am personally using this template all the time.

Feel free to propose suggestions 😉

## 🔨 Usage

1. This is a template repository, which means you can click "Use this template" on GitHub and create your own repo out of it.
1. Then, you need to clone the created repo locally. This is typically achieved by running
    ```bash
    git clone https://github.com/<YourUsername>/<YourRepoName>.git
    ```
    in the terminal. Some people swear by the command line, some by GitHub Desktop, some by Tortoise Git; pick the tool you like.
1. After cloning your repo locally, change the metadata passed to `juce_add_plugin()` function in the *CMakeLists.txt* file. In particular, change
    1. `COMPANY_NAME`
    1. `PLUGIN_MANUFACTURER_CODE`
    1. `PLUGIN_CODE`
    1. `PRODUCT_NAME`
1. After cloning your repo locally, you can proceed with the usual CMake workflow. That involves two steps:
    1. Configure (buildsystem generation)
    1. Build (buildystem invocation)

    In the main repo directory, execute

    ```bash
    cmake --preset default # configure
    cmake --build --preset default # build
    ctest --preset default # test
    ```

    The first run will take the most time because the dependencies (CPM, JUCE, and googletest) need to be downloaded.

    Check [*CMakePresets.json*](./CMakePresets.json) for presets other than "default", feel free to add your own, of course!

4. (Optional) To run clang-format on every commit, in the main directory, execute

    ```bash
    pre-commit install
    ```

    (for this, you need to have `pre-commit` installed, e.g., with `pip`: `pip install pre-commit`).

## 🧱 How was this template built?

See how I created v1 of this template step by step in this video:

[![Audio plugin template tutorial video](http://img.youtube.com/vi/Uq7Hwt18s3s/0.jpg)](https://www.youtube.com/watch?v=Uq7Hwt18s3s "Audio plugin template tutorial video")

## 📃 License

This template repo uses the [Unlicense license](./LICENSE.md) so that you don't have to worry about giving me credit.

If you found the repo helpful, please consider [buying me a coffee](https://buymeacoffee.com/janwilczek).

Remember that CPM, JUCE, and GoogleTest are separately licensed.

