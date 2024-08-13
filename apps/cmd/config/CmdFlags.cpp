#include "CmdFlags.h"
#include "../utils/CliValidator.h"
#include "CLI/App.hpp"
#include "Config.h"
#include "Milody/util/milody_env.h"


namespace Config::Flags {
CLI::Option* singleFileFlag(CLI::App* const s, std::string desc, bool required, bool fileExistingCheck) {
    auto ret = s->add_option("--file,file", Config::file, desc);
    if (required) {
        ret = ret->required(true);
    }
    if (fileExistingCheck) {
        ret = ret->check(CliValidator::ExistingFile);
    }
    return ret;
}

CLI::Option* file1Flag(CLI::App* const s, std::string desc, bool required, bool fileExistingCheck) {
    auto ret = s->add_option("file1", Config::file1, desc);
    if (required) {
        ret = ret->required(true);
    }
    if (fileExistingCheck) {
        ret = ret->check(CliValidator::ExistingFile);
    }
    return ret;
}

CLI::Option* file2Flag(CLI::App* const s, std::string desc, bool required, bool fileExistingCheck) {
    auto ret = s->add_option("file2", Config::file2, desc);
    if (required) {
        ret = ret->required(true);
    }
    if (fileExistingCheck) {
        ret = ret->check(CliValidator::ExistingFile);
    }
    return ret;
}

CLI::Option* fileConfig(CLI::App* const s, std::string desc, bool required, bool fileExistingCheck) {
    auto ret =  s->add_option("-c,--config", Config::fileConfig, desc);
    if (required) {
        ret = ret->required(true);
    }
    if (fileExistingCheck) {
        ret = ret->check(CliValidator::ExistingFile);
    }
    return ret;
}

CLI::Option* srcFileFlag(CLI::App* const s, std::string desc, bool required, bool fileExistingCheck) {
    auto ret =  s->add_option("--src", Config::fileSrc, desc);
    if (required) {
        ret = ret->required(true);
    }
    if (fileExistingCheck) {
        ret = ret->check(CliValidator::ExistingFile);
    }
    return ret;
}

CLI::Option* dstFileFlag(CLI::App* const s, std::string desc, bool required, bool fileExistingCheck) {
    auto ret =  s->add_option("--dst", Config::fileDst, desc);
    if (required) {
        ret = ret->required(true);
    }
    if (fileExistingCheck) {
        ret = ret->check(CliValidator::ExistingFile);
    }
    return ret;
}

CLI::Option* env(CLI::App* const s, std::string desc) {
    return s->add_option("-e,--env", Config::envList, desc);
}
} // namespace Config::Flags
