#include <Milody/audio/milody_juce_format_manager.h>

milody::util::Lazy<juce::AudioFormatManager*> AudioFormatManagerFactory([]() {
    auto mgr = new juce::AudioFormatManager();
    mgr->registerBasicFormats();
    return mgr;
});

juce::AudioFormatManager* milody::audio::GetAudioFormatManager() {
    return AudioFormatManagerFactory.Get();
}
