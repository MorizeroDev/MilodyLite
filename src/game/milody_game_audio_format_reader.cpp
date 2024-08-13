#include "Milody/audio/milody_juce_audio_format_reader.h"
#include <Milody/game/milody_game_interface.h>
#include <Milody/game/milody_game_retcode.h>
#include <Milody/util/milody_strutil.h>

using namespace milody::audio;
int64_t
MilodyAudioJuceAudioFormatReaderCreate(JuceAudioFormatReader*& reader, uint8_t* sourceData, size_t sourceDataSize) {
    reader = new JuceAudioFormatReader(sourceData, sourceDataSize);
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceAudioFormatReaderDestroy(JuceAudioFormatReader*& reader) {
    delete reader;
    reader = nullptr;
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceAudioFormatReaderGetSampleRate(JuceAudioFormatReader* reader, double& sampleRate) {
    sampleRate = reader->getSampleRate();
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceAudioFormatReaderGetBitsPerSample(JuceAudioFormatReader* reader, uint32_t& bitsPerSample) {
    bitsPerSample = reader->getBitsPerSample();
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceAudioFormatReaderGetLengthInSamples(JuceAudioFormatReader* reader, int64_t& lengthInSamples) {
    lengthInSamples = reader->getLengthInSamples();
    return MILODY_API_RET_OK;
}

int64_t MilodyAudioJuceAudioFormatReaderGetNumChannels(JuceAudioFormatReader* reader, uint32_t& numChannels) {
    numChannels = reader->getNumChannels();
    return MILODY_API_RET_OK;
}
