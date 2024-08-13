#ifndef MILODY_JUCE_AUDIO_SOURCE_PLAYER_H
#define MILODY_JUCE_AUDIO_SOURCE_PLAYER_H

#include <Milody/util/milody_time.h>
#include <chrono>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_core/juce_core.h>
#include <memory>


namespace milody::audio {
class JuceAudioSourcePlayer : public JuceRefAudioIODeviceCallback {
public:
    JuceAudioSourcePlayer(float initialAudioSourcePlayerGain);

    ~JuceAudioSourcePlayer();

    MILODY_DECLARE_NON_COPYABLE(JuceAudioSourcePlayer);

    int64_t GetSourceIndexLength();

    double GetSourceTimeLength();

    double GetPlaybackTime();

    int64_t GetPlaybackIndex();

    void SetTransportPositionWithSourceIndexPosition(int64_t position);

    void SetTransportPositionWithSourceTimePosition(double position);

    double GetTransportTimePosition();

    void SetLoop(bool loop);

    bool GetLoop();

    int SetAudioBuffer(JuceAudioBuffer* buffer, int numSampleRate);

    bool LoadFromMemory(const uint8_t* sourceData, size_t sourceDataSize);

    /// <summary>
    /// Set position to zero and start playing
    /// </summary>
    void Start(bool sync);

    /// <summary>
    /// Pause audio on middle
    /// </summary>
    void Pause();

    /// <summary>
    /// Resume from pause(will continue play audio from last pause point)
    /// </summary>
    void Resume(bool sync);

    /// <summary>
    /// Stop audio and set position to zero
    /// </summary>
    void Stop();

    bool IsPlaying();

    bool HasNormalEnd();

    void SetGain(float gain);

    void SetSpeed(float speed, bool sync);

    bool SourceLoaded() {
        return audioSource != nullptr;
    }

    juce::AudioIODeviceCallback* RefAudioIODeviceCallback() override {
        return &outputSourcePlayer;
    }

private:
    // 音频源数据
    std::unique_ptr<juce::PositionableAudioSource> audioSource;

    inline bool noData() {
        return audioSource == nullptr;
    }

    // DSP 中间件
    juce::AudioTransportSource transportSource;

    // 输出播放器
    juce::AudioSourcePlayer outputSourcePlayer;

    double sampleRate = 0;

    milody::util::time::TimePoint lastStartTime;
    double lastStartAtPosition;

    double speed = 1;
    float gain = 1;
    bool isLooping = true;

    void DisposeAudioMemory();
};
} // namespace milody::audio

#endif
