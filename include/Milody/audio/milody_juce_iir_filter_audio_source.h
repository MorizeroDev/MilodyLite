#ifndef MILODY_JUCE_IIR_FILTER_AUDIO_SOURCE_H
#define MILODY_JUCE_IIR_FILTER_AUDIO_SOURCE_H

#include "Milody/util/milody_time.h"
#include <juce_dsp/juce_dsp.h>

namespace milody::audio {

typedef juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>
        IIRFilterType;

class JuceIIRFilterAudioSource : public juce::AudioSource {
private:
    juce::AudioSource* source = nullptr;

    int outputSamplesPerBlockExpected;
    double outputSampleRate;
    uint32_t channels;

    std::shared_ptr<IIRFilterType> fromFilter;
    std::shared_ptr<IIRFilterType> toFilter;

    float translateDurationInSample = 48000;
    float sampleNeedToFill = 0; // 当这个为 0 的时候，表示已经完全到 toFilter 了

public:
    JuceIIRFilterAudioSource(uint32_t channels);

    ~JuceIIRFilterAudioSource() override;

    void setSource(AudioSource* newSource) {
        auto old = source;
        source = newSource;
        if (old != nullptr) {
            old->releaseResources();
        }
    }

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    void releaseResources() override;

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    void ClearApplyIIRFilterState(uint32_t durationInSample);

    void ApplyIIRFilterState(juce::dsp::IIR::Coefficients<float>& state, uint32_t durationInSample);

    float GetFadeOutCoefficients() {
        if (sampleNeedToFill <= 0) {
            return 0;
        }
        if (sampleNeedToFill >= translateDurationInSample) {
            return 1;
        }
        float t_prime = 1 - sampleNeedToFill / translateDurationInSample;
        return log10(10 - 9 * t_prime);
    }

    float GetFadeInCoefficients() {
        if (sampleNeedToFill <= 0) {
            return 1;
        }
        if (sampleNeedToFill >= translateDurationInSample) {
            return 0;
        }
        float t_prime = 1 - sampleNeedToFill / translateDurationInSample;
        return log10(1 + 9 * t_prime);
    }
};
} // namespace milody::audio

#endif // MILODY_JUCE_MUA_AUDIO_SOURCE_H
