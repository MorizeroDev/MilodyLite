#ifndef MILODY_JUCE_AUDIO_BUFFER_H
#define MILODY_JUCE_AUDIO_BUFFER_H

#include <Milody/platform/milody_windows.h>
#include <Milody/util/milody_class.h>
#include <Milody/util/milody_env.h>
#include <Milody/util/milody_time.h>
#include <chrono>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_core/juce_core.h>
#include <memory>


namespace milody::audio {
class JuceAudioBuffer {
public:
    JuceAudioBuffer(int numChannelsToUse, int numSamples, bool mlock) {
        auto size = numChannelsToUse * numSamples * sizeof(float);
        memory = new float[numChannelsToUse * numSamples];

        if (mlock && milody::util::env::getenv<bool>("MILODY_JUCE_AUDIOBUFFER_MLOCK", false)) {
#if WIN32
            if (!milody::platform::VirtualLock(memory, size)) {
                MILODYLOG_ERROR("fail to lock memory, size: {}", size);
            };
#endif
        }

        memoryRef = reinterpret_cast<float**>(malloc(sizeof(float*) * numChannelsToUse));
        for (int i = 0; i < numChannelsToUse; i++) {
            memoryRef[i] = &memory[i * numSamples];
        }

        audioBuffer = juce::AudioBuffer<float>(memoryRef, numChannelsToUse, numSamples);
    };

    ~JuceAudioBuffer() {
        delete memoryRef;
        delete memory;
    };

    MILODY_DECLARE_NON_COPYABLE(JuceAudioBuffer);

    int getNumChannels() const {
        return audioBuffer.getNumChannels();
    }

    int getNumSamples() const {
        return audioBuffer.getNumSamples();
    }

    void writeFloat(const float* pcm) {
        float* const* ptr = audioBuffer.getArrayOfWritePointers();
        int channel_num = getNumChannels();
        int sample_num = getNumSamples();

        if (channel_num == 2) {
            for (int i = 0; i < sample_num; i++) {
                for (int j = 0; j < 2; j++) {
                    ptr[j][i] = pcm[i * 2 + j];
                }
            }
        } else {
            for (int i = 0; i < sample_num; i++) {
                for (int j = 0; j < channel_num; j++) {
                    ptr[j][i] = pcm[i * channel_num + j];
                }
            }
        }
    }

    void writeInt16(const int16_t* pcm) {
        float* const* ptr = audioBuffer.getArrayOfWritePointers();
        int channel_num = getNumChannels();
        int sample_num = getNumSamples();

        if (channel_num == 2) {
            for (int i = 0; i < sample_num; i++) {
                for (int j = 0; j < 2; j++) {
                    ptr[j][i] = pcm[i * 2 + j] / 32768.0;
                }
            }
        } else {
            for (int i = 0; i < sample_num; i++) {
                for (int j = 0; j < channel_num; j++) {
                    ptr[j][i] = pcm[i * channel_num + j] / 32768.0;
                }
            }
        }
    }

    const float* getReaderPointer(int chan) {
        return audioBuffer.getReadPointer(chan);
    }

    juce::AudioBuffer<float>& ref() {
        return audioBuffer;
    }

    juce::AudioBuffer<float>* unwrap() {
        return &audioBuffer;
    }

private:
    // 音频源数据
    juce::AudioBuffer<float> audioBuffer;

    // buffer
    float** memoryRef;

    // buffer
    float* memory;
};
} // namespace milody::audio

#endif
