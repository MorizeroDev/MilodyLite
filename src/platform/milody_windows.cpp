#if WIN32
#include <Milody/platform/milody_windows.h>

#include "Milody/log/log.h"

#include <windows.h>

#include <memoryapi.h>

namespace milody::platform {
bool VirtualLock(void* lpAddress, size_t dwSize) {
    auto ret = ::SetProcessWorkingSetSize(GetCurrentProcess(), 512 * 1024 * 1024, -1);
    if (ret == 0) {
        MILODYLOG_ERROR("fail to SetProcessWorkingSetSize, err: {}", GetLastError());
        return false;
    }

    ret = ::VirtualLock(lpAddress, dwSize);
    if (!ret) {
        MILODYLOG_ERROR("fail to call VirtualLock, err: {}", GetLastError());
        return false;
    }
    return true;
};
} // namespace milody::platform
#endif
