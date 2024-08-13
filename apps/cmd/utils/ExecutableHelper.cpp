#include "ExecutableHelper.h"
#include "CollectionsHelper.h"
#include <Milody/log/log.h>

namespace ExecutableHelper {

std::string getCurrentExecutablePath() {
    char path[4096];

#if defined(_WIN32)
    GetModuleFileName(NULL, path, sizeof(path));
#elif defined(__linux__)
    ssize_t count = readlink("/proc/self/exe", path, sizeof(path));
    if (count != -1) {
        path[count] = '\0';
    }
#elif defined(__APPLE__)
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) != 0) {
        path[0] = '\0'; // Buffer too small
    }
#else
    std::cerr << "Unsupported platform" << std::endl;
    return "";
#endif

    return {path};
}

std::string getCurrentExecutableDirectory() {
    std::string executablePath = getCurrentExecutablePath();
    return std::filesystem::path(executablePath).parent_path().string();
}

int mexec(std::vector<std::string> const& args) {
    auto cmd = CollectionsHelper::join(args);
    MILODYLOG_INFO("================================================================");
    MILODYLOG_INFO("command: {}", cmd);
    return system(cmd.c_str());
}
} // namespace ExecutableHelper
