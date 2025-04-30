#ifndef MILODY_GAME_INTERFACE_H
#define MILODY_GAME_INTERFACE_H

#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>

#ifdef MILODY_BUILDING_ENV

#include "milody_game_types.h"
#include <Milody/common/milody_export_macros.h>

#else

#include "milody_export_macros.h"
#include "milody_game_types.h"

#endif

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-attributes"
#endif

extern "C" {

// 返回值为大版本号 major
MILODY_API int64_t MilodyGetVersion(int32_t& major, int32_t& minor, int32_t& patch);


MILODY_API int64_t MilodyAudioJuceAudioBufferCreate(milody::audio::JuceAudioBuffer*& buffer,
                                                    int32_t numChannelsToUse,
                                                    int32_t numSamples,
                                                    int32_t mlock);
MILODY_API int64_t
MilodyAudioJuceAudioBufferDestroy([[milize::RefType("ref")]] milody::audio::JuceAudioBuffer*& buffer);
MILODY_API int64_t MilodyAudioJuceAudioBufferCreateFromFile(milody::audio::JuceAudioBuffer*& buffer,
                                                            int32_t mlock,
                                                            milody::audio::JuceAudioFormatReader* formatReader);
MILODY_API int64_t MilodyAudioJuceAudioBufferResample(milody::audio::JuceAudioBuffer* buffer,
                                                      double sourceSampleRate,
                                                      milody::audio::JuceAudioBuffer*& result,
                                                      double targetSampleRate);
MILODY_API int64_t MilodyAudioJuceAudioBufferWriteFloat(milody::audio::JuceAudioBuffer* buffer,
                                                        [[milize::CSharpType("float*")]] const float* pcm);
MILODY_API int64_t MilodyAudioJuceAudioBufferWriteInt16(milody::audio::JuceAudioBuffer* buffer,
                                                        [[milize::CSharpType("void*")]] const int16_t* pcm);
MILODY_API int64_t MilodyAudioJuceAudioBufferGetReaderPointer(milody::audio::JuceAudioBuffer* buffer,
                                                              int32_t chan,
                                                              [[milize::CSharpType("float*")]] const float*& ptr);
MILODY_API int64_t MilodyAudioJuceAudioBufferGetNumChannels(milody::audio::JuceAudioBuffer* buffer, int32_t& chan);
MILODY_API int64_t MilodyAudioJuceAudioBufferGetNumSamples(milody::audio::JuceAudioBuffer* buffer, int32_t& samples);

MILODY_API int64_t MilodyAudioJuceDeviceManagerCreate(milody::audio::JuceAudioDeviceManager*& mgr);
MILODY_API int64_t
MilodyAudioJuceDeviceManagerDestroy([[milize::RefType("ref")]] milody::audio::JuceAudioDeviceManager*& mgr);
MILODY_API int64_t MilodyAudioJuceDeviceManagerInitialize(milody::audio::JuceAudioDeviceManager* mgr);
MILODY_API int64_t MilodyAudioJuceDeviceManagerGetAvailableDeviceTypes(milody::audio::JuceAudioDeviceManager* mgr,
                                                                       milody::game::model::BytesWrapper*& result);
MILODY_API int64_t MilodyAudioJuceDeviceManagerGetCurrentAudioDeviceInfo(milody::audio::JuceAudioDeviceManager* mgr,
                                                                         milody::game::model::BytesWrapper*& result);
MILODY_API int64_t
MilodyAudioJuceDeviceManagerGetCurrentAudioDeviceGetLastError(milody::audio::JuceAudioDeviceManager* mgr,
                                                              milody::game::model::BytesWrapper*& result);
MILODY_API int64_t
MilodyAudioJuceDeviceManagerGetCurrentAudioDeviceGetXRunCount(milody::audio::JuceAudioDeviceManager* mgr,
                                                              int32_t& xRunCount);
MILODY_API int64_t MilodyAudioJuceDeviceManagerSetCurrentOutputDeviceType(milody::audio::JuceAudioDeviceManager* mgr,
                                                                          uint8_t* device);
MILODY_API int64_t MilodyAudioJuceDeviceManagerSetCurrentOutputDeviceName(milody::audio::JuceAudioDeviceManager* mgr,
                                                                          uint8_t* devicNamee);
MILODY_API int64_t MilodyAudioJuceDeviceManagerSetCurrentBufferSize(milody::audio::JuceAudioDeviceManager* mgr,
                                                                    int32_t bufferSize);
MILODY_API int64_t MilodyAudioJuceDeviceManagerAddAudioCallback(milody::audio::JuceAudioDeviceManager* mgr,
                                                                milody::audio::JuceRefAudioIODeviceCallback* player);
MILODY_API int64_t MilodyAudioJuceDeviceManagerRemoveAudioCallback(milody::audio::JuceAudioDeviceManager* mgr,
                                                                   milody::audio::JuceRefAudioIODeviceCallback* player);
// clang-format off
MILODY_API int64_t
MilodyAudioJuceDeviceManagerSetChangeListenerCallback(milody::audio::JuceAudioDeviceManager* mgr,
                                                      [[milize::CSharpType("MilodyCTypes.MilodyJuceAudioDeviceManagerChangeListenerCallback?")]] MilodyJuceAudioDeviceManagerChangeListenerCallback callback,
                                                      [[milize::CSharpType("IntPtr")]] void* ctx);
// clang-format on
MILODY_API int64_t MilodyAudioJuceDeviceManagerEnableDaemonCallback(milody::audio::JuceAudioDeviceManager* mgr);
MILODY_API int64_t MilodyAudioJuceDeviceManagerDaemonCallbackGetCount(milody::audio::JuceAudioDeviceManager* mgr,
                                                                      int64_t& result);
MILODY_API int64_t MilodyAudioJuceDeviceManagerDaemonCallbackIsStopped(milody::audio::JuceAudioDeviceManager* mgr,
                                                                       int64_t& result);
MILODY_API int64_t
MilodyAudioJuceDeviceManagerDaemonCallbackGetErrorMessage(milody::audio::JuceAudioDeviceManager* mgr,
                                                          milody::game::model::BytesWrapper*& output);

MILODY_API int64_t MilodyAudioJuceAudioSourcePlayerCreate(milody::audio::JuceAudioSourcePlayer*& player,
                                                          float initialAudioSourcePlayerGain);
MILODY_API int64_t
MilodyAudioJuceAudioSourcePlayerDestroy([[milize::RefType("ref")]] milody::audio::JuceAudioSourcePlayer*& player);
MILODY_API int64_t MilodyAudioJuceAudioSourcePlayerGetSourceTimeLength(milody::audio::JuceAudioSourcePlayer* player,
                                                                       double& length);
MILODY_API int64_t MilodyAudioJuceAudioSourcePlayerGetSourceIndexLength(milody::audio::JuceAudioSourcePlayer* player,
                                                                        int64_t& length);
MILODY_API int64_t MilodyAudioJuceAudioSourcePlayerGetPlaybackTime(milody::audio::JuceAudioSourcePlayer* player,
                                                                   double& playbackTime);
MILODY_API int64_t MilodyAudioJuceAudioSourcePlayerGetPlaybackIndex(milody::audio::JuceAudioSourcePlayer* player,
                                                                    int64_t& playbackTime);
MILODY_API int64_t
MilodyAudioJuceAudioSourcePlayerSetTransportPositionWithSourceTimePosition(milody::audio::JuceAudioSourcePlayer* player,
                                                                           double position);
MILODY_API int64_t MilodyAudioJuceAudioSourcePlayerSetTransportPositionWithSourceIndexPosition(
        milody::audio::JuceAudioSourcePlayer* player,
        int64_t position);
MILODY_API int64_t MilodyAudioJuceAudioSourcePlayerGetLoop(milody::audio::JuceAudioSourcePlayer* player, int32_t& loop);
MILODY_API int64_t MilodyAudioJuceAudioSourcePlayerSetLoop(milody::audio::JuceAudioSourcePlayer* player, int32_t loop);
MILODY_API int64_t MilodyAudioJuceAudioSourcePlayerGetIsPlaying(milody::audio::JuceAudioSourcePlayer* player,
                                                                int32_t& state);
MILODY_API int64_t MilodyAudioJuceAudioSourcePlayerHasNormalEnd(milody::audio::JuceAudioSourcePlayer* player,
                                                                int32_t& state);
MILODY_API int64_t MilodyAudioJuceAudioSourcePlayerSetGain(milody::audio::JuceAudioSourcePlayer* player, float gain);
MILODY_API int64_t MilodyAudioJuceAudioSourcePlayerSetSpeed(milody::audio::JuceAudioSourcePlayer* player,
                                                            float speed,
                                                            int32_t sync);
MILODY_API int64_t MilodyAudioJuceAudioSourcePlayerSourceLoaded(milody::audio::JuceAudioSourcePlayer* player,
                                                                int32_t& loaded);
MILODY_API int64_t MilodyAudioJuceAudioSourcePlayerSetAudioBuffer(milody::audio::JuceAudioSourcePlayer* player,
                                                                  milody::audio::JuceAudioBuffer* buffer,
                                                                  int32_t numSampleRate);
MILODY_API int64_t MilodyAudioJuceAudioSourcePlayerLoadFromMemory(milody::audio::JuceAudioSourcePlayer* player,
                                                                  uint8_t* sourceData,
                                                                  size_t sourceDataSize);
MILODY_API int64_t MilodyAudioJuceAudioSourcePlayerStart(milody::audio::JuceAudioSourcePlayer* player, int32_t sync);
MILODY_API int64_t MilodyAudioJuceAudioSourcePlayerPause(milody::audio::JuceAudioSourcePlayer* player);
MILODY_API int64_t MilodyAudioJuceAudioSourcePlayerResume(milody::audio::JuceAudioSourcePlayer* player, int32_t sync);
MILODY_API int64_t MilodyAudioJuceAudioSourcePlayerStop(milody::audio::JuceAudioSourcePlayer* player);


MILODY_API int64_t MilodyAudioJuceAudioFormatReaderCreate(milody::audio::JuceAudioFormatReader*& reader,
                                                          uint8_t* sourceData,
                                                          size_t sourceDataSize);
MILODY_API int64_t
MilodyAudioJuceAudioFormatReaderDestroy([[milize::RefType("ref")]] milody::audio::JuceAudioFormatReader*& reader);
MILODY_API int64_t MilodyAudioJuceAudioFormatReaderGetSampleRate(milody::audio::JuceAudioFormatReader* reader,
                                                                 double& sampleRate);
MILODY_API int64_t MilodyAudioJuceAudioFormatReaderGetBitsPerSample(milody::audio::JuceAudioFormatReader* reader,
                                                                    uint32_t& bitsPerSample);
MILODY_API int64_t MilodyAudioJuceAudioFormatReaderGetLengthInSamples(milody::audio::JuceAudioFormatReader* reader,
                                                                      int64_t& lengthInSamples);
MILODY_API int64_t MilodyAudioJuceAudioFormatReaderGetNumChannels(milody::audio::JuceAudioFormatReader* reader,
                                                                  uint32_t& numChannels);


MILODY_API int64_t MilodyAudioJuceSoundPlayerCreate(milody::audio::JuceSoundPlayer*& player,
                                                    float initialAudioSourcePlayerGain);
MILODY_API int64_t
MilodyAudioJuceSoundPlayerDestroy([[milize::RefType("ref")]] milody::audio::JuceSoundPlayer*& player);
MILODY_API int64_t MilodyAudioJuceSoundPlayerPlayJuceAudioFormatReader(milody::audio::JuceSoundPlayer* player,
                                                                       milody::audio::JuceAudioFormatReader* reader);
MILODY_API int64_t MilodyAudioJuceSoundPlayerPlayJuceAudioBuffer(milody::audio::JuceSoundPlayer* player,
                                                                 milody::audio::JuceAudioBuffer* reader);
MILODY_API int64_t MilodyAudioJuceSoundPlayerSetGain(milody::audio::JuceSoundPlayer* player, float newGain);


MILODY_API int64_t MilodyContextNativeCancellationTokenContextCreate(milody::context::Context*& ctx);
MILODY_API int64_t
MilodyContextNativeCancellationTokenContextDestroy([[milize::RefType("ref")]] milody::context::Context*& ctx);
MILODY_API int64_t MilodyContextNativeCancellationTokenContextCancel(milody::context::Context* ctx);
// clang-format off
MILODY_API int64_t MilodyContextManagedCancellationTokenContextCreate(milody::context::Context*& ctx,
                                                                      [[milize::CSharpType("MilodyCTypes.MilodyManagedCancellationTokenContextCancelledCallback")]] MilodyManagedCancellationTokenContextCancelledCallback cancelled,
                                                                      [[milize::CSharpType("IntPtr")]] void* cancelledCtx) ;
// clang-format on
MILODY_API int64_t
MilodyContextManagedCancellationTokenContextDestroy([[milize::RefType("ref")]] milody::context::Context*& ctx);

}

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif // MILODY_MILODY_GAME_INTERFACE_H
