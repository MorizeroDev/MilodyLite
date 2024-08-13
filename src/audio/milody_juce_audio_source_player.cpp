#include "Milody/audio/milody_juce_format_manager.h"
#include "Milody/audio/milody_juce_time_slice_thread.h"
#include <Milody/audio/milody_juce_audio_device_manager.h>
#include <Milody/audio/milody_juce_audio_source_player.h>
#include <Milody/log/log.h>
#include <Milody/util/milody_time.h>

milody::audio::JuceAudioSourcePlayer::JuceAudioSourcePlayer(float initialAudioSourcePlayerGain) {
    SetGain(1);
    outputSourcePlayer.setLastGain(initialAudioSourcePlayerGain);

    lastStartTime = std::chrono::high_resolution_clock::now();
    lastStartAtPosition = 0;
    outputSourcePlayer.setSource(&transportSource);
}

milody::audio::JuceAudioSourcePlayer::~JuceAudioSourcePlayer() {
    transportSource.setSource(nullptr);
    outputSourcePlayer.setSource(nullptr);
}

double milody::audio::JuceAudioSourcePlayer::GetPlaybackTime() {
    if (noData()) {
        return -1;
    }

    if (HasNormalEnd()) {
        return static_cast<double>(audioSource->getTotalLength()) / sampleRate;
    } else if (!IsPlaying()) {
        return lastStartAtPosition;
    }
    auto now = std::chrono::high_resolution_clock::now();
    auto time = milody::util::time::GetDuration(now, lastStartTime);
    auto t = lastStartAtPosition + time * speed;
    t = ((double) (static_cast<int64_t>(t * sampleRate) % audioSource->getTotalLength())) / sampleRate;
    return t;
}

int64_t milody::audio::JuceAudioSourcePlayer::GetPlaybackIndex() {
    if (noData()) {
        return -1;
    }
    if (HasNormalEnd()) {
        return audioSource->getTotalLength();
    } else if (!IsPlaying()) {
        return static_cast<int64_t>(lastStartAtPosition * sampleRate); // TODO
    }
    auto now = std::chrono::high_resolution_clock::now();
    auto sampleElapsed = static_cast<int64_t>(lastStartAtPosition * sampleRate +
                                              milody::util::time::GetDuration(now, lastStartTime) * speed * sampleRate);
    return sampleElapsed % audioSource->getTotalLength();
}

int64_t milody::audio::JuceAudioSourcePlayer::GetSourceIndexLength() {
    if (noData()) {
        return -1;
    }
    return audioSource->getTotalLength();
}

double milody::audio::JuceAudioSourcePlayer::GetSourceTimeLength() {
    return static_cast<double>(GetSourceIndexLength()) / sampleRate;
}

void milody::audio::JuceAudioSourcePlayer::SetTransportPositionWithSourceIndexPosition(int64_t position) {
    if (noData()) {
        return;
    }
    //    MILODYLOG_DEBUG("SetTransportPositionWithSourceIndexPosition before:transportSource.nextReadPosition {}",
    //                    transportSource.getNextReadPosition());
    lastStartAtPosition = static_cast<double>(position) / sampleRate;
    lastStartTime = std::chrono::high_resolution_clock::now();
    transportSource.setPosition(lastStartAtPosition / speed);
    //    MILODYLOG_DEBUG("SetTransportPositionWithSourceIndexPosition after:transportSource.nextReadPosition {}",
    //                    transportSource.getNextReadPosition());
}

void milody::audio::JuceAudioSourcePlayer::SetTransportPositionWithSourceTimePosition(double position) {
    if (noData()) {
        return;
    }
    //    MILODYLOG_DEBUG("SetTransportPositionWithSourceTimePosition before:transportSource.getCurrentPosition {}",
    //                    transportSource.getCurrentPosition());
    lastStartAtPosition = position;
    lastStartTime = std::chrono::high_resolution_clock::now();
    transportSource.setPosition(lastStartAtPosition / speed);
    //    MILODYLOG_DEBUG("SetTransportPositionWithSourceTimePosition after:transportSource.getCurrentPosition {}",
    //                    transportSource.getCurrentPosition());
}

double milody::audio::JuceAudioSourcePlayer::GetTransportTimePosition() {
    return transportSource.getCurrentPosition();
}

void milody::audio::JuceAudioSourcePlayer::DisposeAudioMemory() {
    transportSource.stop();
    transportSource.setSource(nullptr);
    audioSource.reset();
}

int milody::audio::JuceAudioSourcePlayer::SetAudioBuffer(milody::audio::JuceAudioBuffer* buffer, int numSampleRate) {
    DisposeAudioMemory();

    if (buffer == nullptr) {
        audioSource = nullptr;
        MILODYLOG_DEBUG("AudioBuffer Cleared");
        return 0;
    }

    audioSource = std::make_unique<juce::MemoryAudioSource>(buffer->ref(), false, false);
    transportSource.setSource(audioSource.get(), 32768, GetTimeSliceThread(), numSampleRate);
    audioSource->setLooping(isLooping);
    sampleRate = numSampleRate;
    SetTransportPositionWithSourceTimePosition(0);
    return 0;
}

bool milody::audio::JuceAudioSourcePlayer::LoadFromMemory(const uint8_t* sourceData, size_t sourceDataSize) {
    DisposeAudioMemory();

    juce::AudioFormatReader* reader = GetAudioFormatManager()->createReaderFor(
            std::make_unique<juce::MemoryInputStream>(sourceData, sourceDataSize, true));

    if (reader == nullptr) {
        return false;
    }

    audioSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
    transportSource.setSource(audioSource.get(), 32768, GetTimeSliceThread(), reader->sampleRate);
    audioSource->setLooping(isLooping);
    sampleRate = reader->sampleRate;
    SetTransportPositionWithSourceTimePosition(0);
    return true;
}

void milody::audio::JuceAudioSourcePlayer::Start(bool sync) {
    if (noData()) {
        return;
    }

    if (!IsPlaying()) {
        transportSource.setPosition(0);
        transportSource.start();
        if (sync) {
            transportSource.waitUntilActuallyPlaying();
        }
        lastStartTime = std::chrono::high_resolution_clock::now();
        lastStartAtPosition = 0;
    }
}

void milody::audio::JuceAudioSourcePlayer::Pause() {
    if (noData()) {
        return;
    }

    if (IsPlaying()) {
        //        MILODYLOG_DEBUG("before pause: {}", GetPlaybackIndex());
        lastStartAtPosition = GetPlaybackTime();
        transportSource.stop();
        //        MILODYLOG_DEBUG("after pause: {}", GetPlaybackIndex());
    }
}

void milody::audio::JuceAudioSourcePlayer::Resume(bool sync) {
    if (noData()) {
        return;
    }

    if (!IsPlaying()) {
        //        MILODYLOG_DEBUG("before resume: {}", GetPlaybackIndex());
        transportSource.setPosition(lastStartAtPosition / speed); // speed ratio
        transportSource.start();
        if (sync) {
            transportSource.waitUntilActuallyPlaying();
        }
        lastStartTime = std::chrono::high_resolution_clock::now();
        //        MILODYLOG_DEBUG("after resume: {}", GetPlaybackIndex());
    }
}

void milody::audio::JuceAudioSourcePlayer::Stop() {
    if (noData()) {
        return;
    }
    lastStartTime = std::chrono::high_resolution_clock::now();
    lastStartAtPosition = 0;
    transportSource.stop();
    transportSource.setPosition(0);
}

bool milody::audio::JuceAudioSourcePlayer::IsPlaying() {
    if (noData()) {
        return false;
    }

    return transportSource.isPlaying();
}

bool milody::audio::JuceAudioSourcePlayer::HasNormalEnd() {
    if (noData()) {
        return false;
    }

    return transportSource.hasStreamFinished();
}

void milody::audio::JuceAudioSourcePlayer::SetGain(float _gain) {
    gain = _gain;
    transportSource.setGain(_gain);
}

void milody::audio::JuceAudioSourcePlayer::SetSpeed(float newSpeed, bool sync) {
    if (noData()) {
        speed = newSpeed;
        return;
    }

    auto playing = IsPlaying();
    if (playing) {
        Pause();
    }
    transportSource.setSource(audioSource.get(), 32768, GetTimeSliceThread(), newSpeed * sampleRate);
    speed = newSpeed;
    if (playing) {
        Resume(sync);
    }
    //    MILODYLOG_DEBUG("after speed: {}", GetPlaybackTime());
}

void milody::audio::JuceAudioSourcePlayer::SetLoop(bool loop) {
    isLooping = loop;
    if (!noData()) {
        audioSource->setLooping(isLooping);
    }
}

bool milody::audio::JuceAudioSourcePlayer::GetLoop() {
    return isLooping;
}
