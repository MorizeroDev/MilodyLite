#ifndef MILODY_STRUTIL_H
#define MILODY_STRUTIL_H

#include <cstring>
#include <string>

namespace milody::util {
inline size_t copyStringToBuffer(const std::string& str, uint8_t* buffer, size_t bufferSize) {
    auto resultCStr = str.c_str();
    auto len = str.length();
    if (len > bufferSize - 1) {
        len = bufferSize - 1;
    }
    memset(buffer, 0, bufferSize);
    memcpy(buffer, resultCStr, len);
    return len;
}
} // namespace milody::util

#endif
