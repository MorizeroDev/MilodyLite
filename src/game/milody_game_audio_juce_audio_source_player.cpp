#include <Milody/audio/milody_juce_audio_device_manager.h>
#include <Milody/game/milody_game_interface.h>
#include <Milody/game/milody_game_retcode.h>
#include <Milody/log/log.h>
#include <Milody/util/milody_strutil.h>

using namespace milody::audio;

int64_t MilodyAudioJuceAudioSourcePlayerCreate(JuceAudioSourcePlayer*& player, float initialAudioSourcePlayerGain) {
    player = nullptr;
    player = new JuceAudioSourcePlayer(initialAudioSourcePlayerGain);
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceAudioSourcePlayerDestroy(JuceAudioSourcePlayer*& player) {
    delete player;
    player = nullptr;
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceAudioSourcePlayerGetSourceTimeLength(JuceAudioSourcePlayer* player, double& length) {
    length = player->GetSourceTimeLength();
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceAudioSourcePlayerGetSourceIndexLength(JuceAudioSourcePlayer* player, int64_t& length) {
    length = player->GetSourceIndexLength();
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceAudioSourcePlayerGetPlaybackTime(JuceAudioSourcePlayer* player, double& playbackTime) {
    playbackTime = player->GetPlaybackTime();
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceAudioSourcePlayerGetPlaybackIndex(JuceAudioSourcePlayer* player, int64_t& playbackTime) {
    playbackTime = player->GetPlaybackIndex();
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceAudioSourcePlayerSetTransportPositionWithSourceTimePosition(JuceAudioSourcePlayer* player,
                                                                                   double position) {
    player->SetTransportPositionWithSourceTimePosition(position);
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceAudioSourcePlayerSetTransportPositionWithSourceIndexPosition(JuceAudioSourcePlayer* player,
                                                                                    int64_t position) {
    player->SetTransportPositionWithSourceIndexPosition(position);
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceAudioSourcePlayerGetLoop(JuceAudioSourcePlayer* player, int32_t& loop) {
    loop = player->GetLoop() ? 1 : 0;
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceAudioSourcePlayerSetLoop(JuceAudioSourcePlayer* player, int32_t loop) {
    player->SetLoop(loop != 0);
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceAudioSourcePlayerSetAudioBuffer(JuceAudioSourcePlayer* player,
                                                       milody::audio::JuceAudioBuffer* buffer,
                                                       int32_t numSampleRate) {
    return player->SetAudioBuffer(buffer, numSampleRate);
}

int64_t MilodyAudioJuceAudioSourcePlayerLoadFromMemory(JuceAudioSourcePlayer* player,
                                                       uint8_t* sourceData,
                                                       size_t sourceDataSize) {
    if (player->LoadFromMemory(sourceData, sourceDataSize)) {
        return MILODY_API_RET_OK;
    }
    MILODYLOG_DEBUG("load audio data failed");
    return MILODY_API_RET_AUDIO_FAILED;
}

int64_t MilodyAudioJuceAudioSourcePlayerStart(JuceAudioSourcePlayer* player, int32_t sync) {
    player->Start(sync != 0);
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceAudioSourcePlayerPause(JuceAudioSourcePlayer* player) {
    player->Pause();
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceAudioSourcePlayerResume(JuceAudioSourcePlayer* player, int32_t sync) {
    player->Resume(sync != 0);
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceAudioSourcePlayerStop(JuceAudioSourcePlayer* player) {
    player->Stop();
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceAudioSourcePlayerGetIsPlaying(JuceAudioSourcePlayer* player, int32_t& state) {
    state = reinterpret_cast<JuceAudioSourcePlayer*>(player)->IsPlaying() ? 1 : 0;
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceAudioSourcePlayerHasNormalEnd(JuceAudioSourcePlayer* player, int32_t& state) {
    state = reinterpret_cast<JuceAudioSourcePlayer*>(player)->HasNormalEnd() ? 1 : 0;
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceAudioSourcePlayerSetGain(JuceAudioSourcePlayer* player, float gain) {
    player->SetGain(gain);
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceAudioSourcePlayerSetSpeed(JuceAudioSourcePlayer* player, float speed, int32_t sync) {
    player->SetSpeed(speed, sync != 0);
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceAudioSourcePlayerSourceLoaded(JuceAudioSourcePlayer* player, int32_t& loaded) {
    loaded = player->SourceLoaded() ? 1 : 0;
    return MILODY_API_RET_OK;
}
