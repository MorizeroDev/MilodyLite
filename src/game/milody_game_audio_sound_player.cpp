#include "Milody/audio/milody_juce_sound_player.h"
#include <Milody/audio/milody_juce_audio_device_manager.h>
#include <Milody/game/milody_game_interface.h>
#include <Milody/game/milody_game_retcode.h>
#include <Milody/util/milody_strutil.h>

using namespace milody::audio;
int64_t MilodyAudioJuceSoundPlayerCreate(JuceSoundPlayer*& player, float initialAudioSourcePlayerGain) {
    player = new JuceSoundPlayer(initialAudioSourcePlayerGain);
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceSoundPlayerDestroy(JuceSoundPlayer*& player) {
    delete player;
    player = nullptr;
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceSoundPlayerPlayJuceAudioFormatReader(JuceSoundPlayer* player, JuceAudioFormatReader* reader) {
    player->play(reader);
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceSoundPlayerPlayJuceAudioBuffer(JuceSoundPlayer* player, JuceAudioBuffer* reader) {
    player->play(reader);
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceSoundPlayerSetGain(JuceSoundPlayer* player, float newGain) {
    player->setGain(newGain);
    return MILODY_API_RET_OK;
}
