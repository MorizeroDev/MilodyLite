#ifndef MILODY_GAME_RETCODE_H
#define MILODY_GAME_RETCODE_H

#include <cstdint>

constexpr int64_t MILODY_API_RET_OK = 0;
constexpr int64_t MILODY_API_RET_UNEXPECTED_EXCEPTION = -11709394;
constexpr int64_t MILODY_API_RET_FAILED = -1;

constexpr int64_t MILODY_API_RET_AUDIO_FAILED = -1;

constexpr int64_t MILODY_API_RET_CANCELED = -100;     // 传入的HANDLE错误

constexpr int64_t MILODY_API_RET_INITIALIZED = 1;

#endif //MILODY_MILODY_GAME_RETCODE_H
