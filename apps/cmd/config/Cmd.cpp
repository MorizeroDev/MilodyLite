#include "../biz/Audio.h"
#include "CLI/App.hpp"
#include "CmdFlags.h"

namespace Config::Cmd {
std::function<void()> DefaultHelp(CLI::App* app) {
    return [app]() {
        if (!app) {
            std::cout << app->help() << std::endl;
        }
    };
}

namespace audio {

} // namespace audio

void audioRoute(CLI::App* app) {
    auto cmd = app->add_subcommand("audio", "Audio Tools");
    cmd->callback(DefaultHelp(cmd));
}

} // namespace Config::Cmd
