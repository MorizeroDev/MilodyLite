#include "Milody/audio/milody_juce_audio_format_reader.h"
#include <Milody/audio/milody_juce_audio_device_manager.h>
#include <Milody/game/milody_game_interface.h>
#include <Milody/game/milody_game_retcode.h>
#include <Milody/log/log.h>

using namespace milody::audio;
int64_t MilodyAudioJuceAudioBufferCreate(JuceAudioBuffer*& buffer,
                                         int32_t numChannel,
                                         int32_t lengthPreChannel,
                                         int32_t mlock) {
    buffer = new JuceAudioBuffer(numChannel, lengthPreChannel, mlock != 0);
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceAudioBufferDestroy(JuceAudioBuffer*& buffer) {
    delete buffer;
    buffer = nullptr;
    return MILODY_API_RET_OK;
}

int64_t
MilodyAudioJuceAudioBufferCreateFromFile(JuceAudioBuffer*& buffer, int32_t mlock, JuceAudioFormatReader* formatReader) {
    buffer = new JuceAudioBuffer(static_cast<int>(formatReader->unwrap()->numChannels),
                                 static_cast<int>(formatReader->unwrap()->lengthInSamples),
                                 mlock != 0);

    bool ok = formatReader->unwrap()->read(buffer->ref().getArrayOfWritePointers(),
                                           buffer->getNumChannels(),
                                           0,
                                           buffer->getNumSamples());
    if (!ok) {
        MILODYLOG_ERROR("Failed to read audio data into JuceAudioBuffer");
        return MILODY_API_RET_AUDIO_FAILED;
    }

    return MILODY_API_RET_OK;
}

int64_t
MilodyAudioJuceAudioBufferResample(JuceAudioBuffer* buffer, double sourceSampleRate, JuceAudioBuffer*& result, double targetSampleRate) {
    auto audioSource = juce::MemoryAudioSource(buffer->ref(), false, false);

    juce::ResamplingAudioSource resampler(&audioSource, false, buffer->getNumChannels());
    resampler.setResamplingRatio(sourceSampleRate / targetSampleRate);
    resampler.prepareToPlay(static_cast<int>(sourceSampleRate), targetSampleRate);

    int numSamples = static_cast<int>(buffer->getNumSamples() * targetSampleRate / sourceSampleRate);
    auto milodyAudioBuffer= new milody::audio::JuceAudioBuffer(buffer->getNumChannels(), numSamples, false);

    // Fill the resampled buffer
    juce::AudioSourceChannelInfo resampledChannelInfo(milodyAudioBuffer->unwrap(), 0, milodyAudioBuffer->unwrap()->getNumSamples());
    resampler.getNextAudioBlock(resampledChannelInfo);

    result = milodyAudioBuffer;
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceAudioBufferWriteFloat(JuceAudioBuffer* buffer, const float* pcm) {
    buffer->writeFloat(pcm);
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceAudioBufferWriteInt16(JuceAudioBuffer* buffer, const int16_t* pcm) {
    buffer->writeInt16(pcm);
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceAudioBufferGetReaderPointer(JuceAudioBuffer* buffer, int32_t chan, const float*& ptr) {
    ptr = buffer->getReaderPointer(chan);
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceAudioBufferGetNumChannels(JuceAudioBuffer* buffer, int32_t& chan) {
    chan = buffer->getNumChannels();
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceAudioBufferGetNumSamples(JuceAudioBuffer* buffer, int32_t& samples) {
    samples = buffer->getNumSamples();
    return MILODY_API_RET_OK;
}
