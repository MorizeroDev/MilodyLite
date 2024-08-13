#ifndef MILODY_AUDIOTYPE_H
#define MILODY_AUDIOTYPE_H
#include <Milody/common/milody_result.h>
#include <Milody/util/milody_serializerable.h>
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <unordered_set>

namespace milody::audio {

inline std::string getListOfActiveBits(const juce::BigInteger& b) {
    juce::StringArray bits;

    for (int i = 0; i <= b.getHighestBit(); ++i)
        if (b[i])
            bits.add(juce::String(i));

    return std::string(bits.joinIntoString(", ").toRawUTF8());
}

inline std::vector<std::string> toVector(const juce::StringArray& data) {
    std::vector<std::string> ret;
    for (const auto& item: data) {
        ret.push_back(std::string(item.toRawUTF8()));
    }
    return ret;
}

template <typename T>
inline std::vector<T> toVector(const juce::Array<T>& data) {
    std::vector<T> ret;
    for (auto item: data) {
        ret.push_back(item);
    }
    return ret;
}

class AudioIODeviceTypeInfo : public milody::util::serialization::serializable {
public:
    AudioIODeviceTypeInfo(juce::AudioIODeviceType* const pType);

    bool empty = false;

    std::vector<std::string> deviceNames;
    std::string typeName;

    inline nlohmann::json toJson() override {
        if (empty) {
            return  nlohmann::json(nullptr);
        }

        return {
                {"type_name", typeName},
                {"device_names", deviceNames},
        };
    };
};

class AudioIODeviceInfo : public milody::util::serialization::serializable {
public:
    AudioIODeviceInfo(juce::AudioIODevice* const p);

    bool empty = false;

    std::string name;
    std::string typeName;
    std::vector<std::string> outputChannelNames;
    std::vector<std::string> inputChannelNames;
    std::optional<juce::BigInteger> defaultOutputChannels;
    std::optional<juce::BigInteger> defaultInputChannels;
    std::vector<double> availableSampleRates;
    std::vector<int> availableBufferSizes;
    int defaultBufferSize{};
    bool isOpen{};
    bool isPlaying{};
    int currentBufferSizeSamples{};
    double currentSampleRate{};
    int currentBitDepth{};
    juce::BigInteger activeOutputChannels;
    juce::BigInteger activeInputChannels;
    int outputLatencyInSamples{};
    int inputLatencyInSamples{};

    inline nlohmann::json toJson() override {
        if (empty) {
            return  nlohmann::json(nullptr);
        }

        nlohmann::json ret = {
                {"name", name},
                {"type_name", typeName},
                {"output_channel_names", milody::util::serialization::vectorToJson(outputChannelNames)},
                {"input_channel_names", milody::util::serialization::vectorToJson(inputChannelNames)},

                {"available_sample_rates", milody::util::serialization::vectorToJson(availableSampleRates)},
                {"available_buffer_sizes", milody::util::serialization::vectorToJson(availableBufferSizes)},
                {"default_buffer_size", defaultBufferSize},
                {"is_open", isOpen},
                {"is_playing", isPlaying},
                {"current_buffer_size_samples", currentBufferSizeSamples},
                {"current_sample_rate", currentSampleRate},
                {"current_bit_depth", currentBitDepth},
                {"active_output_channels", std::string(activeOutputChannels.toString(10).toRawUTF8())},
                {"active_input_channels", std::string(activeInputChannels.toString(10).toRawUTF8())},
                {"output_latency_in_samples", outputLatencyInSamples},
                {"input_latency_in_samples", inputLatencyInSamples},
        };

        if (defaultOutputChannels.has_value()) {
            ret["default_output_channels"] = std::string(defaultOutputChannels->toString(10).toRawUTF8());
        }
        if (defaultInputChannels.has_value()) {
            ret["default_input_channels"] = std::string(defaultInputChannels->toString(10).toRawUTF8());
        }

        return ret;
    };
};

class JuceRefAudioIODeviceCallback {
public:
    virtual juce::AudioIODeviceCallback* RefAudioIODeviceCallback() = 0;
};

} // namespace milody::audio

#endif
