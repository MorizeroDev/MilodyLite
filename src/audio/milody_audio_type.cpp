#include <Milody/audio/milody_audio_types.h>

milody::audio::AudioIODeviceTypeInfo::AudioIODeviceTypeInfo(juce::AudioIODeviceType* const pType) {
    if (pType == nullptr) {
        empty = true;
        return;
    }

    empty = false;
    deviceNames = milody::audio::toVector(pType->getDeviceNames());
    typeName = std::string(pType->getTypeName().toRawUTF8());
}

milody::audio::AudioIODeviceInfo::AudioIODeviceInfo(juce::AudioIODevice* const p) {
    if (p == nullptr) {
        empty = true;
        return;
    }

    empty = false;
    name = p->getName().toRawUTF8();
    typeName = p->getTypeName().toRawUTF8();
    outputChannelNames = milody::audio::toVector(p->getOutputChannelNames());

    inputChannelNames = milody::audio::toVector(p->getInputChannelNames());
    defaultOutputChannels = p->getDefaultOutputChannels();
    defaultInputChannels = p->getDefaultInputChannels();
    availableSampleRates = milody::audio::toVector(p->getAvailableSampleRates());
    availableBufferSizes = milody::audio::toVector(p->getAvailableBufferSizes());
    defaultBufferSize = p->getDefaultBufferSize();
    isOpen = p->isOpen();
    isPlaying = p->isPlaying();
    currentBufferSizeSamples = p->getCurrentBufferSizeSamples();
    currentSampleRate = p->getCurrentSampleRate();
    currentBitDepth = p->getCurrentBitDepth();
    activeOutputChannels = p->getActiveOutputChannels();
    activeInputChannels = p->getActiveInputChannels();
    outputLatencyInSamples = p->getOutputLatencyInSamples();
    inputLatencyInSamples = p->getInputLatencyInSamples();
}
