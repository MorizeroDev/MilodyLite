#ifndef MILODYCLI_UTILS_CLIVALIDATOR_H
#define MILODYCLI_UTILS_CLIVALIDATOR_H

#include "CLI/Validators.hpp"
#include "CliValidator.h"
#include "Milody/io/milody_io.h"
#include <filesystem>
#include <string>

namespace CliValidator {
class ExistingFileValidator : public CLI::Validator {
public:
    ExistingFileValidator();
};

const ExistingFileValidator ExistingFile;
} // namespace CliValidator


#endif //MILODYCLI_UTILS_CLIVALIDATOR_H
