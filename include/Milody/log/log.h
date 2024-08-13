#ifndef MILODY_LOG_H
#define MILODY_LOG_H

// 使用该头文件则必须依赖于spdlog

#include <algorithm>
#include <memory>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <Milody/common/milody_export_macros.h>

// 直接用以下宏

#define MILODYLOG_TRACE(...) milody::log::MilodyLogger::getInstance().getLogger()->trace(__VA_ARGS__)

#define MILODYLOG_DEBUG(...) milody::log::MilodyLogger::getInstance().getLogger()->debug(__VA_ARGS__)

#define MILODYLOG_INFO(...) milody::log::MilodyLogger::getInstance().getLogger()->info(__VA_ARGS__)

#define MILODYLOG_WARN(...) milody::log::MilodyLogger::getInstance().getLogger()->warn(__VA_ARGS__)

#define MILODYLOG_ERROR(...) milody::log::MilodyLogger::getInstance().getLogger()->error(__VA_ARGS__)

#define MILODYLOG_CRITICAL(...) milody::log::MilodyLogger::getInstance().getLogger()->critical(__VA_ARGS__)

// 避免直接包含unity头文件
typedef struct IUnityInterface IUnityInterface;
typedef struct IUnityLog IUnityLog;

namespace milody::log {

// 自定义UnityLogSink
template <typename Mutex>
class UnityLogSink : public spdlog::sinks::base_sink<Mutex> {
public:
    void init(IUnityLog* log);

protected:
    void sink_it_(const spdlog::details::log_msg& msg) override;
    void flush_() override;

private:
    IUnityLog* unityLog{};
};

using UnityLogSink_mt = UnityLogSink<std::mutex>;

// 管理sink和logger，支持控制台，文件，unity
class MILODY_API MilodyLogger {
public:
    static MilodyLogger& getInstance();

    static void init();
    static void initWithUnity(IUnityLog* log);

    void addConsoleSink();
    void addFileSink(const std::string& filename);
    void addUnityLogSink(IUnityLog* log);

    std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> getConsoleSink() const;
    std::shared_ptr<spdlog::sinks::basic_file_sink_mt> getFileSink() const;
    std::shared_ptr<UnityLogSink_mt> getUnityLogSink() const;

    static void enableSink(const std::shared_ptr<spdlog::sinks::sink>& sink);
    static void disableSink(const std::shared_ptr<spdlog::sinks::sink>& sink);

    spdlog::logger* getLogger() const;
    void setLevel(spdlog::level::level_enum level);

    MilodyLogger(const MilodyLogger&) = delete;
    MilodyLogger& operator=(const MilodyLogger&) = delete;

private:
    MilodyLogger();
    ~MilodyLogger() = default;

    std::shared_ptr<spdlog::logger> logger;
    std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> consoleSink;
    std::shared_ptr<spdlog::sinks::basic_file_sink_mt> fileSink;
    std::shared_ptr<UnityLogSink_mt> unitySink;
};

} // namespace milody::log

#endif //MILODY_LOG_H
