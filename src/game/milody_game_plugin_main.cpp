#include <IUnityLog.h>
#include <Milody/game/milody_game_interface.h>
#include <Milody/log/log.h>

static IUnityLog* unityLogPtr = nullptr;

extern "C" {
UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API UnityPluginLoad(IUnityInterfaces* unityInterfacesPtr) {
    unityLogPtr = unityInterfacesPtr->Get<IUnityLog>();
    milody::log::MilodyLogger::initWithUnity(unityLogPtr);
    MILODYLOG_DEBUG("Milody Launched");
}

UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API UnityPluginUnload() {
    unityLogPtr = nullptr;
}

MILODY_API int64_t MilodyGetVersion(int32_t& major, int32_t& minor, int32_t& patch) {
    major = MILODY_VERSION_MAJOR;
    minor = MILODY_VERSION_MINOR;
    patch = MILODY_VERSION_PATCH;

    MILODYLOG_DEBUG("GetVersion: major={}, minor={}, patch={}", major, minor, patch);
    return MILODY_VERSION_MAJOR;
}
}
