/*
==============================================================================

BEGIN_JUCE_MODULE_DECLARATION

   ID:            audio_plugin
   vendor:        WolfSound
   version:       0.1.0
   name:          Audio Plugin
   description:   Plugin core
   dependencies:  juce_audio_utils, juce_dsp

   website:       https://thewolfsound.com
   license:       Unlicense

END_JUCE_MODULE_DECLARATION

==============================================================================
*/

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include <ranges>
#include <vector>
#include <wolfsound/dsp/wolfsound_FractionalDelayLine.hpp>
#include <wolfsound/common/wolfsound_Frequency.hpp>
#include <wolfsound/common/wolfsound_assert.hpp>
#include <fx/fx.h>
#include "include/PluginProcessor.h"
#include "include/PluginEditor.h"

// #include all additional header files below
