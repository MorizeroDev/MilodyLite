#include <Milody/audio/milody_audio_types.h>
#include <Milody/audio/milody_juce_audio_device_manager.h>
#include <Milody/audio/milody_juce_audio_source_player.h>
#include <Milody/log/log.h>
#include <juce_core/juce_core.h>

milody::audio::JuceAudioDeviceManager::JuceAudioDeviceManager() {
    MILODYLOG_DEBUG("Creating JuceAudioDeviceManager");

    auto message = audioDeviceManager.initialise(0, 2, nullptr, true, {}, nullptr);
    if (message.length() != 0) {
        MILODYLOG_ERROR(message.toRawUTF8());
        throw std::runtime_error("fail to initialize JuceAudioDeviceManager");
    }

    audioDeviceManager.addChangeListener(this);
    audioDeviceManager.getAudioDeviceSetup(_deviceSetup);
    MILODYLOG_DEBUG("JuceAudioDeviceManager Created");
}

milody::audio::JuceAudioDeviceManager::~JuceAudioDeviceManager() {
    audioDeviceManager.removeChangeListener(this);
    MILODYLOG_DEBUG("JuceAudioDeviceManager Destructed");
}

void milody::audio::JuceAudioDeviceManager::initialize() {
    auto message = audioDeviceManager.initialise(0, 2, nullptr, true, {}, nullptr);
    if (message.length() != 0) {
        MILODYLOG_ERROR(message.toRawUTF8());
        throw std::runtime_error("fail to initialize JuceAudioDeviceManager");
    }
    MILODYLOG_DEBUG("JuceAudioDeviceManager initialized");
}

void milody::audio::JuceAudioDeviceManager::setChangeListenerCallback(
        MilodyJuceAudioDeviceManagerChangeListenerCallback callback,
        void* ctx) {
    managedChangeListenerCallback = callback;
    managedChangeListenerCallbackCtx = ctx;
}

void milody::audio::JuceAudioDeviceManager::changeListenerCallback(juce::ChangeBroadcaster* source) {
    if (managedChangeListenerCallback != nullptr) {
        managedChangeListenerCallback(managedChangeListenerCallbackCtx);
    }
}

std::vector<milody::audio::AudioIODeviceTypeInfo> milody::audio::JuceAudioDeviceManager::GetAvailableDeviceTypes() {
    std::vector<AudioIODeviceTypeInfo> ret;
    auto& types = audioDeviceManager.getAvailableDeviceTypes();
    for (auto& item: types) {
        AudioIODeviceTypeInfo res(item);
        ret.push_back(res);
    }
    return ret;
}

milody::audio::AudioIODeviceInfo milody::audio::JuceAudioDeviceManager::GetCurrentAudioDevice() {
    return audioDeviceManager.getCurrentAudioDevice();
}

void milody::audio::JuceAudioDeviceManager::SetCurrentOutputDeviceType(std::string device) {
    audioDeviceManager.setCurrentAudioDeviceType(device, true);
}

Result<void, std::string> milody::audio::JuceAudioDeviceManager::SetCurrentOutputDeviceName(std::string deviceName) {
    audioDeviceManager.getAudioDeviceSetup(_deviceSetup);
    _deviceSetup.outputDeviceName = juce::String::fromUTF8(deviceName.c_str(), deviceName.length());
    auto message = audioDeviceManager.setAudioDeviceSetup(_deviceSetup, true);

    if (message.length() != 0) {
        auto s = std::string(message.toRawUTF8());
        MILODYLOG_ERROR(s);
        return Err(s);
    }
    return Ok();
}
