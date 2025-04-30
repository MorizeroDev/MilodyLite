#ifndef MILODY_JUCE_DAEMON_AUDIO_SOURCE_H
#define MILODY_JUCE_DAEMON_AUDIO_SOURCE_H

#include <juce_audio_devices/juce_audio_devices.h>

namespace milody::audio {

class JuceDaemonIODeviceCallback : public juce::AudioIODeviceCallback {
private:
    int64_t counter = 0;
    bool stopped = false;
    std::string errorMessage = {};

public:
    JuceDaemonIODeviceCallback();

    ~JuceDaemonIODeviceCallback() override;

    void audioDeviceIOCallbackWithContext(const float* const* inputChannelData,
                                          int numInputChannels,
                                          float* const* outputChannelData,
                                          int numOutputChannels,
                                          int numSamples,
                                          const juce::AudioIODeviceCallbackContext& context) override;

     void audioDeviceAboutToStart(juce::AudioIODevice* device) override;

     void audioDeviceStopped() override;

     void audioDeviceError(const juce::String& errorMessage) override;

     int64_t getCount() {
         return counter;
     }

     bool isStopped() {
         return stopped;
     }

     std::string getErrorMessage() {
         return errorMessage;
     }
};

} // namespace milody::audio

#endif //MILODY_JUCE_DAEMON_AUDIO_SOURCE_H
