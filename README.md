# Beginner’s Guide to C++ Audio Programming with the JUCE Framework

## C++ Online 2026 Workshop by Jan Wilczek

This is the companion repository for the C++ Online 2026 workshop.

Register here: https://cpponline.uk/workshop/jumpstart-to-cpp-in-audio/

## Repository structure

```
cpp-online-26-workshop/
├── audio_plugin/            # JUCE audio plugin code
│   ├── assets/              # binary assets (fonts)
│   ├── include/             # headers
│   └── source/              # implementation
├── cmake/                   # CMake utilities
├── data/                    # audio test files
├── docs/
│   ├── slides/              # workshop presentation slides
│   └── tasks/               # 👈 workshop task descriptions
├── fx/                      # header-only effects library
├── music_player/            # command-line music player app
├── CMakeLists.txt           # Root CMake configuration
└── CMakePresets.json        # CMake presets (Debug/Release)
```

## 📃 License

This repo uses the [MIT License](./LICENSE.md).

Remember that JUCE and GoogleTest are separately licensed.
