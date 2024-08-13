#include "Milody/audio/milody_juce_iir_filter_audio_source.h"

using namespace milody::audio;

JuceIIRFilterAudioSource::JuceIIRFilterAudioSource(uint32_t channels) {
    this->channels = channels;
};

JuceIIRFilterAudioSource::~JuceIIRFilterAudioSource(){

};

void JuceIIRFilterAudioSource::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    this->outputSamplesPerBlockExpected = samplesPerBlockExpected;
    this->outputSampleRate = sampleRate;

    fromFilter = nullptr;
    toFilter = nullptr;
};

void JuceIIRFilterAudioSource::releaseResources() {
    // nop
};

std::unique_ptr<juce::AudioBuffer<float>>
copyAudioBufferFromAudioSourceChannelInfo(const juce::AudioSourceChannelInfo& bufferToFill) {
    // 获取 bufferToFill 中的通道数、起始样本位置和样本数量
    int numChannels = bufferToFill.buffer->getNumChannels();
    int startSample = bufferToFill.startSample;
    int numSamples = bufferToFill.numSamples;

    // 创建一个新的 AudioBuffer<float> 对象
    auto newBuffer = std::make_unique<juce::AudioBuffer<float>>(numChannels, numSamples);

    // 复制 bufferToFill 中的数据到 newBuffer 中
    for (int channel = 0; channel < numChannels; ++channel) {
        // 获取 bufferToFill 中当前通道的数据指针
        const float* sourceData = bufferToFill.buffer->getReadPointer(channel, startSample);

        // 获取 newBuffer 中当前通道的数据指针
        float* destData = newBuffer->getWritePointer(channel, 0);

        // 复制数据
        std::copy(sourceData, sourceData + numSamples, destData);
    }
    return std::move(newBuffer);
}

void iirProcess(juce::dsp::AudioBlock<float>& audioBlock, IIRFilterType* iirFilter) {
    if (iirFilter == nullptr) {
        return;
    }
    juce::dsp::ProcessContextReplacing<float> context(audioBlock);
    iirFilter->process(context);
};

void iirProcess(juce::AudioBuffer<float>* audioBuffer, IIRFilterType* iirFilter) {
    if (iirFilter == nullptr) {
        return;
    }
    juce::dsp::AudioBlock<float> audioBlock(audioBuffer->getArrayOfWritePointers(),
                                            audioBuffer->getNumChannels(),
                                            audioBuffer->getNumSamples());
    iirProcess(audioBlock, iirFilter);
};

void JuceIIRFilterAudioSource::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
    source->getNextAudioBlock(bufferToFill);

    if (sampleNeedToFill <= 0) {
        juce::dsp::AudioBlock<float> outputBlock(bufferToFill.buffer->getArrayOfWritePointers(),
                                                 bufferToFill.buffer->getNumChannels(),
                                                 bufferToFill.buffer->getNumSamples());

        iirProcess(outputBlock, toFilter.get());
        return;
    }

    auto fadeOutBuffer = copyAudioBufferFromAudioSourceChannelInfo(bufferToFill);
    iirProcess(fadeOutBuffer.get(), fromFilter.get());
    auto fadeInBuffer = copyAudioBufferFromAudioSourceChannelInfo(bufferToFill);
    iirProcess(fadeInBuffer.get(), toFilter.get());

    for (int chan = 0; chan < bufferToFill.buffer->getNumChannels(); chan++) {
        auto fadeOutPtr = fadeOutBuffer->getReadPointer(chan);
        auto fadeInPtr = fadeInBuffer->getReadPointer(chan);
        auto mixPtr = bufferToFill.buffer->getWritePointer(chan, bufferToFill.startSample);

        for (int i = 0; i < bufferToFill.numSamples; i++) {
            auto fadeOutVal = fadeOutPtr[i] * GetFadeOutCoefficients();
            auto fadeInVal = fadeInPtr[i] * GetFadeInCoefficients();
            mixPtr[i] = fadeOutVal + fadeInVal;
            sampleNeedToFill--;
            //            MILODYLOG_DEBUG("JuceIIRFilterAudioSource sampleNeedToFill:{}", sampleNeedToFill);
        }
    }
};

void JuceIIRFilterAudioSource::ClearApplyIIRFilterState(uint32_t durationInSample) {
    auto newFromFilter = toFilter;
    auto newToFilter = nullptr;

    if (sampleNeedToFill > 0) {
        // TODO 不好处理，就不处理了
        MILODYLOG_INFO("switch iir filter with sampleNeedToFill gr than 0");
    }

    this->fromFilter = newFromFilter;
    this->toFilter = newToFilter;
    this->translateDurationInSample = durationInSample;
    this->sampleNeedToFill = durationInSample;
}

void JuceIIRFilterAudioSource::ApplyIIRFilterState(juce::dsp::IIR::Coefficients<float>& state,
                                                   uint32_t durationInSample) {
    juce::dsp::ProcessSpec dspPs{
            .sampleRate = outputSampleRate,
            .maximumBlockSize = static_cast<uint32_t>(outputSamplesPerBlockExpected),
            .numChannels = channels,
    };

    //    if (sampleNeedToFill == 0) {
    auto newFromFilter = toFilter;
    auto newToFilter = std::make_shared<IIRFilterType>();
    newToFilter->prepare(dspPs);
    *newToFilter->state = state;
    //    }

    if (sampleNeedToFill > 0) {
        // TODO 不好处理，就不处理了
        MILODYLOG_INFO("switch iir filter with sampleNeedToFill gr than 0");
    }

    this->fromFilter = newFromFilter;
    this->toFilter = newToFilter;
    this->translateDurationInSample = durationInSample;
    this->sampleNeedToFill = durationInSample;
}
