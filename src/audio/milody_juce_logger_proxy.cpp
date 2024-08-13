#include "Milody/util/milody_class.h"
#include <Milody/log/log.h>
#include <juce_core/juce_core.h>

namespace milody::audio {

class ProxiedLogger : public juce::Logger {
public:
    ProxiedLogger() {
        setCurrentLogger(this);
    }

    ~ProxiedLogger() {
        setCurrentLogger(nullptr);
    }

    MILODY_DECLARE_NON_COPYABLE(ProxiedLogger);

protected:
    void logMessage(const juce::String& message) override {
        MILODYLOG_INFO(message.toRawUTF8());
    }
};

static ProxiedLogger logger;

void JugeLoggerInitialize() {
    juce::Logger::setCurrentLogger(&logger);
}

} // namespace milody::audio