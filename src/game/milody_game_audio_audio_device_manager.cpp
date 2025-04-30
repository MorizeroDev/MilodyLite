#include <Milody/audio/milody_juce_audio_device_manager.h>
#include <Milody/audio/milody_juce_logger_proxy.h>
#include <Milody/game/milody_game_interface.h>
#include <Milody/game/milody_game_model.h>
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
    mgr->setChangeListenerCallback(nullptr, nullptr);
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
                                                            milody::game::model::BytesWrapper*& result) {
    result = nullptr;
    auto devices = mgr->GetAvailableDeviceTypes();
    auto json = vectorToJson(devices).dump();
    result = new milody::game::model::BytesWrapper(std::move(json));
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceDeviceManagerGetCurrentAudioDeviceInfo(JuceAudioDeviceManager* mgr,
                                                              milody::game::model::BytesWrapper*& result) {
    result = nullptr;
    auto info = mgr->GetCurrentAudioDeviceInfo();
    auto json = info.toJson().dump();
    result = new milody::game::model::BytesWrapper(std::move(json));
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceDeviceManagerGetCurrentAudioDeviceGetLastError(JuceAudioDeviceManager* mgr,
                                                                      milody::game::model::BytesWrapper*& result) {
    result = nullptr;
    auto info = mgr->GetCurrentAudioDevice();
    if (info == nullptr) {
        return MILODY_API_RET_FAILED;
    }
    auto lastError = info->getLastError();
    result = new milody::game::model::BytesWrapper(lastError.toStdString());
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceDeviceManagerGetCurrentAudioDeviceGetXRunCount(JuceAudioDeviceManager* mgr, int32_t& xRunCount) {
    auto info = mgr->GetCurrentAudioDevice();
    if (info == nullptr) {
        return MILODY_API_RET_FAILED;
    }
    xRunCount = info->getXRunCount();
    return MILODY_API_RET_OK;
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

int64_t MilodyAudioJuceDeviceManagerSetCurrentBufferSize(JuceAudioDeviceManager* mgr, int32_t bufferSize) {
    auto err = mgr->SetCurrentBufferSize(bufferSize);
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

int64_t MilodyAudioJuceDeviceManagerEnableDaemonCallback(JuceAudioDeviceManager* mgr) {
    mgr->EnableDaemonCallback();
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceDeviceManagerDaemonCallbackGetCount(JuceAudioDeviceManager* mgr, int64_t& result) {
    result = mgr->DaemonCallbackGetCount();
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceDeviceManagerDaemonCallbackIsStopped(JuceAudioDeviceManager* mgr, int64_t& result) {
    result = mgr->DaemonCallbackIsStopped() ? 1 : 0;
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceDeviceManagerDaemonCallbackGetErrorMessage(JuceAudioDeviceManager* mgr,
                                                                  milody::game::model::BytesWrapper*& output) {
    auto result = mgr->DaemonCallbackGetErrorMessage();
    output = new milody::game::model::BytesWrapper(result);
    return MILODY_API_RET_OK;
}
