#include <Milody/log/log.h>
#include <Milody/log/log_c.h>
#include <spdlog/spdlog.h>

#include <IUnityLog.h>

// 在加载dll时会默认初始化
[[maybe_unused]] static int MilodyLoggerInitHelper = ([] {
    milody::log::MilodyLogger::init();
}(),
                                                      0);

namespace milody::log {
void MilodyLogger::init() {
    MilodyLogger::getInstance().addConsoleSink();
    // 注意，不支持中文路径
    //    MilodyLogger::getInstance().addFileSink("./milodylog.txt");
}

void MilodyLogger::initWithUnity(IUnityLog* log) {
    MilodyLogger::getInstance().addConsoleSink();
    //    MilodyLogger::getInstance().addFileSink("./milodylog.txt");
    MilodyLogger::getInstance().addUnityLogSink(log);
}

MilodyLogger::MilodyLogger() {
    logger = std::make_shared<spdlog::logger>("Milody");
    logger->set_level(spdlog::level::debug);
}

MilodyLogger& MilodyLogger::getInstance() {
    static MilodyLogger instance;
    return instance;
}

void MilodyLogger::addConsoleSink() {
    if (!consoleSink) {
        consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        consoleSink->set_level(spdlog::level::debug);
        logger->sinks().push_back(consoleSink);
    }
}

void MilodyLogger::addFileSink(const std::string& filename) {
    if (!fileSink) {
        fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(filename, true);
        fileSink->set_level(spdlog::level::trace);
        logger->sinks().push_back(fileSink);
    }
}

void MilodyLogger::addUnityLogSink(IUnityLog* log) {
    if (!unitySink) {
        unitySink = std::make_shared<UnityLogSink_mt>();
        unitySink->init(log);
        unitySink->set_level(spdlog::level::trace);
        logger->sinks().push_back(unitySink);
    }
}

std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> MilodyLogger::getConsoleSink() const {
    return consoleSink;
}

std::shared_ptr<spdlog::sinks::basic_file_sink_mt> MilodyLogger::getFileSink() const {
    return fileSink;
}

std::shared_ptr<UnityLogSink_mt> MilodyLogger::getUnityLogSink() const {
    return unitySink;
}

void MilodyLogger::setLevel(spdlog::level::level_enum level) {
    logger->set_level(level);
}

void MilodyLogger::enableSink(const std::shared_ptr<spdlog::sinks::sink>& sink) {
    sink->set_level(spdlog::level::trace);
}

void MilodyLogger::disableSink(const std::shared_ptr<spdlog::sinks::sink>& sink) {
    sink->set_level(spdlog::level::off);
}

spdlog::logger* MilodyLogger::getLogger() const {
    return logger.get();
}
} // namespace milody::log

extern "C" {

MILODY_API int32_t MilodyLog_EnableUnityLog() {
    auto sink = milody::log::MilodyLogger::getInstance().getUnityLogSink();
    milody::log::MilodyLogger::enableSink(sink);
    return 0;
}

MILODY_API int32_t MilodyLog_DisableUnityLog() {
    auto sink = milody::log::MilodyLogger::getInstance().getUnityLogSink();
    milody::log::MilodyLogger::disableSink(sink);
    return 0;
}

MILODY_API int32_t MilodyLog_EnableConsole() {
    auto sink = milody::log::MilodyLogger::getInstance().getConsoleSink();
    milody::log::MilodyLogger::enableSink(sink);
    return 0;
}

MILODY_API int32_t MilodyLog_DisableConsole() {
    auto sink = milody::log::MilodyLogger::getInstance().getConsoleSink();
    milody::log::MilodyLogger::disableSink(sink);
    return 0;
}

MILODY_API int32_t MilodyLog_EnableFile() {
    auto sink = milody::log::MilodyLogger::getInstance().getFileSink();
    milody::log::MilodyLogger::enableSink(sink);
    return 0;
}

MILODY_API int32_t MilodyLog_DisableFile() {
    auto sink = milody::log::MilodyLogger::getInstance().getFileSink();
    milody::log::MilodyLogger::disableSink(sink);
    return 0;
}
}
