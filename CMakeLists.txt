# Mandatory line, sets the minimum version of CMake that should be used with this repository.
# 3.22 is the minimum CMake version required by JUCE.
# 3.25 is required for the MSVC_DEBUG_INFORMATION_FORMAT feature to work.
# To verify your version run
# $ cmake --version
cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

# Enable setting CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
set(CMAKE_POLICY_DEFAULT_CMP0141
    NEW
    CACHE STRING "" FORCE
)

# Sets the minimum deployment target
# macOS 10.11 is the minimum required by JUCE
set(CMAKE_OSX_DEPLOYMENT_TARGET
    "10.11"
    CACHE STRING "" FORCE
)
# Builds a binary that works on Apple Silicon (arm64) and Intel-based Macs
set(CMAKE_OSX_ARCHITECTURES
    "x86_64;arm64"
    CACHE STRING "" FORCE
)

# Sets a few variables, like PROJECT_NAME. VERSION is needed by JUCE.
project(AudioPlugin)

# Set the C++ standard only if this project is standalone;
# otherwise, let clients decide.
if(PROJECT_IS_TOP_LEVEL)
  if(NOT CMAKE_CXX_STANDARD)
    # Always use the newest C++ standard on green-field projects if possible.
    set(CMAKE_CXX_STANDARD 23)
  endif()

  # Links to C++ runtime statically
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  # Embeds debug information
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
      "$<$<CONFIG:Debug,RelWithDebInfo>:Embedded>"
      CACHE STRING "" FORCE
  )
endif()

# I like to download the dependencies to the same folder as the project.
# If you want to install them system wide, set CPM_SOURCE_CACHE with the path to the dependencies
# either as an environment variable or pass it to the cmake script with -DCPM_SOURCE_CACHE=<path>.
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)

# Downloads CPM if not already downloaded. CPM is an easy-to-use package manager nicely integrated with CMake.
include(cmake/cpm.cmake)

# This commands downloads AND configures JUCE. It sets up some variables, like JUCE_SOURCE_DIR.
cpmaddpackage(
  NAME
  JUCE
  GIT_TAG
  8.0.12
  GITHUB_REPOSITORY
  juce-framework/JUCE
  SOURCE_DIR
  ${LIB_DIR}/juce
)

# Adds googletest.
cpmaddpackage(
  NAME
  GOOGLETEST
  GITHUB_REPOSITORY
  google/googletest
  VERSION
  1.17.0
  SOURCE_DIR
  ${LIB_DIR}/googletest
  OPTIONS
  "INSTALL_GTEST OFF"
  "gtest_force_shared_crt ON"
)

# Add compiler warning utilities
include(cmake/CompilerWarnings.cmake)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  # Sets the Windows VST3 installation location to one
  # that doesn't require elevated permissions
  set_directory_properties(PROPERTIES JUCE_VST3_COPY_DIR "$ENV{LOCALAPPDATA}/Programs/Common/VST3")
endif()

# Adds a plugin target (that's basically what the Projucer does).
juce_add_plugin(
  AudioPlugin
  COMPANY_NAME
  "MyCompany" # change this
  IS_SYNTH
  FALSE # may change this
  NEEDS_MIDI_INPUT
  FALSE # may change this
  NEEDS_MIDI_OUTPUT
  FALSE # may change this
  PLUGIN_MANUFACTURER_CODE
  Mcmp # change this (for max compatibility, first letter must be uppercase, remaining lowercase)
  PLUGIN_CODE
  Expl # change this (for max compatibility, first letter must be uppercase, remaining lowercase)
  FORMATS
  AAX
  AU
  Standalone
  VST3
  PRODUCT_NAME
  "YourPluginName" # change this
  VERSION
  0.0.0 # 0.0.0 forces AU host to always rescan the plugin. Change this before releasing
  COPY_PLUGIN_AFTER_BUILD
  FALSE # Changing this to TRUE will enable automatic copying of the built plugin formats
        # to correct directories on your system. The directories must exist, though
)

# These definitions are recommended by JUCE
target_compile_definitions(AudioPlugin PRIVATE JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JUCE_VST3_CAN_REPLACE_VST2=0)

# We want to have a "library" target with PluginProcessor and PluginEditor to use in plugins and in tests.
# Library targets that use JUCE modules non-PRIVATEly should be created as JUCE module themselves,
# not as static or shared libraries. Otherwise, you risk buggy JUCE build configuration.
# This is unlikely on small projects, but we want to use the best practices from the start
# (and migrating to JUCE modules at a late development stage is a nightmare, trust me).
juce_add_module(audio_plugin ALIAS_NAMESPACE my_company)

# In the following, we re-export some of the compile-time definitions derived from arguments of
# juce_add_plugin(). This is necessary to maintain consistent behavior in built plugins and in tests.
# Check juce/extras/Build/CMake/JUCEUtils.cmake to see how they are defined for the AudioPlugin target.
# Alternatively, you could use
# target_compile_definitions(AudioPluginTest PRIVATE $<TARGET_PROPERTY:AudioPlugin,COMPILE_DEFINITIONS>)
# in the test/CMakeLists.txt file, but I find it more risky to blindly export all compile
# definitions for all targets using audio_plugin
# (1. you can forget to do it 2. you don't know what you're defining)
# than to re-export the meaningful ones for audio_plugin and just link against the module to use it.
target_compile_definitions(
  audio_plugin
  INTERFACE JUCE_PLUGIN_NAME="$<TARGET_PROPERTY:AudioPlugin,JUCE_PLUGIN_NAME>"
            JUCE_IS_MIDI_EFFECT=$<BOOL:$<TARGET_PROPERTY:AudioPlugin,JUCE_IS_MIDI_EFFECT>>
            JUCE_IS_SYNTH=$<BOOL:$<TARGET_PROPERTY:AudioPlugin,JUCE_IS_SYNTH>>
            JUCE_NEEDS_MIDI_INPUT=$<BOOL:$<TARGET_PROPERTY:AudioPlugin,JUCE_NEEDS_MIDI_INPUT>>
            JUCE_NEEDS_MIDI_OUTPUT=$<BOOL:$<TARGET_PROPERTY:AudioPlugin,JUCE_NEEDS_MIDI_OUTPUT>>
)

# Enables strict C++ warnings and treats warnings as errors.
target_compile_options(audio_plugin INTERFACE "${PROJECT_WARNINGS_CXX}")

# We need to link the plugin targets with the module containing our code.
target_link_libraries(
  AudioPlugin
  PRIVATE my_company::audio_plugin
  PUBLIC juce::juce_recommended_config_flags juce::juce_recommended_lto_flags juce::juce_recommended_warning_flags
)

# This command allows running tests from the "build" folder (the one where CMake generates the project to).
enable_testing()

# Adds all the targets configured in the "test" folder.
add_subdirectory(test)
