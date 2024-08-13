#include "CliValidator.h"
#include "../config/Config.h"
#include "CLI/Validators.hpp"
#include "Milody/io/milody_io.h"
#include <string>

CliValidator::ExistingFileValidator::ExistingFileValidator() : CLI::Validator("FILE") {
    func_ = [](std::string& filename) {
        if (filename.length() == 0) {
            return std::string("Empty path");
        }

        auto path_result = milody::io::isFileExists(filename);
        if (path_result == milody::io::FileStatus::FileSystemError) {
            return std::string("Fs Error");
        }
        if (path_result == milody::io::FileStatus::NotExists) {
            return "File does not exist: " + filename;
        }
        if (path_result == milody::io::FileStatus::Directory) {
            return "File is actually a directory: " + filename;
        }
        return std::string();
    };
}
