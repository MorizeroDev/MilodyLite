#ifndef MILODY_GAME_TYPES_H
#define MILODY_GAME_TYPES_H

#ifdef MILODY_BUILDING_ENV

#include <Milody/common/milody_export_macros.h>

#else

#include "milody_export_macros.h"

#endif


namespace milody {

namespace audio {
namespace format {
struct MuaPCM;
}

class JuceAudioBuffer;

class JuceAudioSourcePlayer;

class JuceMUASourcePlayer;

class JuceAudioFormatReader;

class JuceSoundPlayer;

class JuceRefAudioIODeviceCallback;

class JuceAudioDeviceManager;
} // namespace audio

namespace context {
class Context;
} // namespace context

} // namespace milody

typedef void (*MilodyJuceAudioDeviceManagerChangeListenerCallback)(const void* ctx);

typedef int (*MilodyManagedCancellationTokenContextCancelledCallback)(const void* ctx);

#endif
