#ifndef MILODYCLI_IO_H
#define MILODYCLI_IO_H

#include <Milody/common/milody_export_macros.h>
#include <cstdint>
#include <string>
#include <vector>

namespace milody::io {
enum class FileStatus {
    FileSystemError,
    Exists,
    NotExists,
    Directory,
};

MILODY_API FileStatus isFileExists(const std::string& path);

MILODY_API std::vector<uint8_t> readFile(const std::string& path);

MILODY_API void writeFile(const std::string& path, const std::vector<uint8_t>& data);

MILODY_API void writeFile(const std::string& path, const uint8_t* data, size_t size);
} // namespace milody::io


#endif //MILODYCLI_UTILS_IO_H
