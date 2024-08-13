#ifndef MILODY_JUCE_AUDIO_DEVICE_MANAGER_H
#define MILODY_JUCE_AUDIO_DEVICE_MANAGER_H

#include "Milody/game/milody_game_types.h"
#include "Milody/util/milody_class.h"
#include "milody_audio_types.h"
#include "milody_juce_audio_buffer.h"
#include "milody_juce_audio_source_player.h"
#include <Milody/common/milody_result.h>
#include <Milody/util/milody_serializerable.h>
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <unordered_set>

namespace milody::audio {
class JuceAudioDeviceManager : juce::ChangeListener {
public:
    JuceAudioDeviceManager();

    ~JuceAudioDeviceManager();

    // MILODY_DECLARE_NON_COPYABLE(JuceAudioDeviceManager); // juce 里已经删掉了复制构造函数了

    void initialize();

    std::vector<AudioIODeviceTypeInfo> GetAvailableDeviceTypes();

    AudioIODeviceInfo GetCurrentAudioDevice();

    void SetCurrentOutputDeviceType(std::string device);

    [[nodiscard]] Result<void, std::string> SetCurrentOutputDeviceName(std::string deviceName);

    void setChangeListenerCallback(MilodyJuceAudioDeviceManagerChangeListenerCallback callback, void* ctx);

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    void AddAudioCallback(juce::AudioIODeviceCallback* newCallback) {
        audioDeviceManager.addAudioCallback(newCallback);
    };

    void RemoveAudioCallback(juce::AudioIODeviceCallback* callback) {
        audioDeviceManager.removeAudioCallback(callback);
    };

private:
    juce::AudioDeviceManager audioDeviceManager;

    juce::AudioDeviceManager::AudioDeviceSetup _deviceSetup;

    MilodyJuceAudioDeviceManagerChangeListenerCallback managedChangeListenerCallback = nullptr;
    void* managedChangeListenerCallbackCtx = nullptr;
};
} // namespace milody::audio

#endif // !MILODY_JUCE_AUDIO_DEVICE_MANAGER_H
