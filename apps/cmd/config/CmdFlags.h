#ifndef MILODYCLI_CONFIG_CMDFLAGS_H
#define MILODYCLI_CONFIG_CMDFLAGS_H

#include "CLI/App.hpp"


namespace Config::Flags {
CLI::Option* singleFileFlag(CLI::App* const s, std::string desc, bool required, bool fileExistingCheck);

CLI::Option* file1Flag(CLI::App* const s, std::string desc, bool required, bool fileExistingCheck);

CLI::Option* file2Flag(CLI::App* const s, std::string desc, bool required, bool fileExistingCheck);

CLI::Option* fileConfig(CLI::App* const s, std::string desc, bool required, bool fileExistingCheck);

CLI::Option* srcFileFlag(CLI::App* const s, std::string desc, bool required, bool fileExistingCheck);

CLI::Option* dstFileFlag(CLI::App* const s, std::string desc, bool required, bool fileExistingCheck);

CLI::Option* env(CLI::App* s, std::string desc);
} // namespace Config::Flags

#endif //MILODYCLI_CONFIG_CMDFLAGS_H
