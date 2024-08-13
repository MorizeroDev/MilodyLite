#ifndef MILODY_JUCE_SOUND_PLAYER_H
#define MILODY_JUCE_SOUND_PLAYER_H

#include "Milody/util/milody_class.h"
#include "milody_juce_audio_format_reader.h"
#include <Milody/audio/milody_juce_sound_player_wrapper.h>
#include <Milody/util/milody_time.h>
#include <chrono>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_core/juce_core.h>
#include <memory>

namespace milody::audio {
class JuceSoundPlayer : public JuceRefAudioIODeviceCallback {
public:
    JuceSoundPlayer(float initialAudioSourcePlayerGain) {
        soundPlayer.refPlayer().setLastGain(initialAudioSourcePlayerGain);
    };

    ~JuceSoundPlayer(){};

    MILODY_DECLARE_NON_COPYABLE(JuceSoundPlayer);

    void play(JuceAudioFormatReader* p) {
        soundPlayer.play(p->unwrap(), false);
    };

    void play(JuceAudioBuffer* p) {
        soundPlayer.play(p->unwrap(), false, true);
    };

    void setGain(float newGain) {
        soundPlayer.refPlayer().setGain(newGain);
    }

    juce::AudioIODeviceCallback* RefAudioIODeviceCallback() override {
        return &soundPlayer;
    }

private:
    juce::SoundPlayer soundPlayer;
};
} // namespace milody::audio

#endif