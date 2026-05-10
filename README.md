# Beginner’s Guide to C++ Audio Programming with the JUCE Framework

## C++ Online 2026 Workshop by Jan Wilczek

This is the companion repository for the C++ Online 2026 workshop.

Register here: https://cpponline.uk/workshop/jumpstart-to-cpp-in-audio/

## Building the project

Check [_docs/tasks/task0.md_](docs/tasks/task0.md) for details.

```bash
cmake --preset default # generate the buildsystem
cmake --build --preset default # build the project
```

## Repository structure

```
cpp-online-26-workshop/
├── audio_plugin/            # audio plugin code (for parts 2 & 3)
│   ├── assets/              # binary assets (fonts)
│   ├── include/             # headers
│   └── source/              # implementation
├── cmake/                   # CMake utilities
├── data/                    # audio test files
├── docs/
│   ├── slides/              # workshop presentation slides
│   └── tasks/               # 👈 workshop task descriptions
├── fx/                      # header-only effects library (for part 1)
├── music_player/            # command-line music player app (for part 1)
├── CMakeLists.txt
└── CMakePresets.json
```

## 📃 License

This repo uses the [MIT License](./LICENSE.md).

Remember that JUCE and GoogleTest are separately licensed.
