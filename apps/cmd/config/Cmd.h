#ifndef MILODYCLI_CONFIG_CMD_H
#define MILODYCLI_CONFIG_CMD_H

#include "CLI/App.hpp"

namespace Config::Cmd {
std::function<void()> DefaultHelp(CLI::App* app);

void audioRoute(CLI::App* app);
} // namespace Config::Cmd


#endif //MILODYCLI_CONFIG_CMD_H
