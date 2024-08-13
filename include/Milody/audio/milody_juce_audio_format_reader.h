#ifndef MILODY_JUCE_AUDIO_FORMAT_READER_H
#define MILODY_JUCE_AUDIO_FORMAT_READER_H

#include "Milody/util/milody_class.h"
#include "juce_audio_formats/juce_audio_formats.h"
#include "milody_juce_audio_device_manager.h"
#include "milody_juce_format_manager.h"

namespace milody::audio {
class JuceAudioFormatReader {
public:
    JuceAudioFormatReader(uint8_t* sourceData, size_t sourceDataSize) {
        formatReader = GetAudioFormatManager()->createReaderFor(
                std::make_unique<juce::MemoryInputStream>(sourceData, sourceDataSize, true));
        if (formatReader == nullptr) {
            throw std::runtime_error("fail to load sound");
        }
    };

    ~JuceAudioFormatReader() {
        delete formatReader;
    };

    MILODY_DECLARE_NON_COPYABLE(JuceAudioFormatReader);

    juce::AudioFormatReader* unwrap() {
        return formatReader;
    }

    double getSampleRate() {
        return formatReader->sampleRate;
    }

    uint32_t getBitsPerSample() {
        return formatReader->bitsPerSample;
    }

    int64_t getLengthInSamples() {
        return formatReader->lengthInSamples;
    }

    uint32_t getNumChannels() {
        return formatReader->numChannels;
    }

private:
    juce::AudioFormatReader* formatReader;
};
} // namespace milody::audio

#endif
