#ifndef MILODY_EXECUTABLEHELPER_H
#define MILODY_EXECUTABLEHELPER_H

#include <filesystem>
#include <string>
#include <vector>

#if defined(_WIN32)

#include <windows.h>

#elif defined(__linux__)

#include <unistd.h>

#elif defined(__APPLE__)

#include <mach-o/dyld.h>

#endif

namespace ExecutableHelper {

std::string getCurrentExecutablePath();

std::string getCurrentExecutableDirectory();

int mexec(std::vector<std::string> const& args);
} // namespace ExecutableHelper
#endif //MILODY_EXECUTABLEHELPER_H
