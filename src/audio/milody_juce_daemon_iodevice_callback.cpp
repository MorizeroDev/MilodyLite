#include "Milody/log/log.h"
#include <Milody/audio/milody_juce_daemon_iodevice_callback.h>

milody::audio::JuceDaemonIODeviceCallback::JuceDaemonIODeviceCallback() {
}

milody::audio::JuceDaemonIODeviceCallback::~JuceDaemonIODeviceCallback() {
}

void milody::audio::JuceDaemonIODeviceCallback::audioDeviceIOCallbackWithContext(
        const float* const* inputChannelData,
        int numInputChannels,
        float* const* outputChannelData,
        int numOutputChannels,
        int numSamples,
        const juce::AudioIODeviceCallbackContext& context) {
    for (int i = 0; i < numOutputChannels; ++i) {
        if (outputChannelData[i] != nullptr) {
            juce::zeromem(outputChannelData[i], (size_t) numSamples * sizeof(float));
        }
    }
    counter += numSamples;
    stopped = false;
};

void milody::audio::JuceDaemonIODeviceCallback::audioDeviceAboutToStart(juce::AudioIODevice* device) {
    counter = 0;
};

void milody::audio::JuceDaemonIODeviceCallback::audioDeviceStopped() {
    stopped = true;
};

void milody::audio::JuceDaemonIODeviceCallback::audioDeviceError(const juce::String& errorMessage) {
    MILODYLOG_ERROR("daemon io device callback, audio device error triggered, {}", errorMessage.toStdString());
    this->errorMessage = errorMessage.toStdString();
};
