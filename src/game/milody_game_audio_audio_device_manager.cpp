#include <Milody/audio/milody_juce_audio_device_manager.h>
#include <Milody/audio/milody_juce_logger_proxy.h>
#include <Milody/game/milody_game_interface.h>
#include <Milody/game/milody_game_retcode.h>
#include <Milody/util/milody_strutil.h>
#include <juce_events/juce_events.h>

using namespace milody::audio;

int64_t MilodyAudioJuceDeviceManagerCreate(JuceAudioDeviceManager*& mgr) try {
    milody::audio::JugeLoggerInitialize();
    (void) juce::MessageManager::getInstance();
    mgr = new JuceAudioDeviceManager();
    return MILODY_API_RET_OK;
} catch (std::runtime_error& e) {
    MILODYLOG_ERROR("{}, {}", __func__, e.what());
    return MILODY_API_RET_AUDIO_FAILED;
} catch (...) {
    MILODYLOG_ERROR("{}, {}", __func__, "unexpected exception");
    return MILODY_API_RET_AUDIO_FAILED;
}

int64_t MilodyAudioJuceDeviceManagerDestroy(JuceAudioDeviceManager*& mgr) {
    delete mgr;
    mgr = nullptr;
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceDeviceManagerInitialize(JuceAudioDeviceManager* mgr) try {
    mgr->initialize();
    return MILODY_API_RET_OK;
} catch (std::runtime_error& e) {
    MILODYLOG_ERROR("{}, {}", __func__, e.what());
    return MILODY_API_RET_AUDIO_FAILED;
} catch (...) {
    MILODYLOG_ERROR("{}, {}", __func__, "unexpected exception");
    return MILODY_API_RET_AUDIO_FAILED;
}

int64_t MilodyAudioJuceDeviceManagerGetAvailableDeviceTypes(JuceAudioDeviceManager* mgr,
                                                            uint8_t* buffer,
                                                            uint64_t bufferSize,
                                                            uint64_t& needed) {
    auto devices = mgr->GetAvailableDeviceTypes();
    auto result = vectorToJson(devices).dump();
    needed = result.size();
    return static_cast<int64_t>(milody::util::copyStringToBuffer(result, buffer, bufferSize));
}

int64_t MilodyAudioJuceDeviceManagerGetCurrentAudioDevice(JuceAudioDeviceManager* mgr,
                                                          uint8_t* buffer,
                                                          uint64_t bufferSize,
                                                          uint64_t& needed) {
    auto info = mgr->GetCurrentAudioDevice();
    auto result = info.toJson().dump();
    needed = result.size();
    return static_cast<int64_t>(milody::util::copyStringToBuffer(result, buffer, bufferSize));
}

int64_t MilodyAudioJuceDeviceManagerSetCurrentOutputDeviceType(JuceAudioDeviceManager* mgr, uint8_t* device) {
    mgr->SetCurrentOutputDeviceType(reinterpret_cast<char*>(device));
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceDeviceManagerSetCurrentOutputDeviceName(JuceAudioDeviceManager* mgr, uint8_t* devicNamee) {
    auto err = mgr->SetCurrentOutputDeviceName(reinterpret_cast<char*>(devicNamee));
    if (err.isErr()) {
        return MILODY_API_RET_AUDIO_FAILED;
    }
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceDeviceManagerAddAudioCallback(JuceAudioDeviceManager* mgr,
                                                     JuceRefAudioIODeviceCallback* player) {
    if (player == nullptr) {
        MILODYLOG_INFO("[MilodyAudioJuceDeviceManagerAddAudioCallback] try to add a null player");
        return MILODY_API_RET_OK;
    }
    mgr->AddAudioCallback(player->RefAudioIODeviceCallback());
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceDeviceManagerRemoveAudioCallback(JuceAudioDeviceManager* mgr,
                                                        JuceRefAudioIODeviceCallback* player) {
    if (player == nullptr) {
        MILODYLOG_INFO("[MilodyAudioJuceDeviceManagerAddAudioCallback] try to remove a null player");
        return MILODY_API_RET_OK;
    }
    mgr->RemoveAudioCallback(player->RefAudioIODeviceCallback());
    return MILODY_API_RET_OK;
}

int64_t
MilodyAudioJuceDeviceManagerSetChangeListenerCallback(JuceAudioDeviceManager* mgr,
                                                      MilodyJuceAudioDeviceManagerChangeListenerCallback callback,
                                                      void* ctx) {
    mgr->setChangeListenerCallback(callback, ctx);
    return MILODY_API_RET_OK;
}
