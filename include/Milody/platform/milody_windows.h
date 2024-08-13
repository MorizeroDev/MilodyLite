#ifndef MILODY_WINDOWS_H
#define MILODY_WINDOWS_H

#if WIN32
namespace milody::platform {
bool VirtualLock(
        void* lpAddress,
        size_t dwSize
);
}
#endif

#endif //MILODY_WINDOWS_H
