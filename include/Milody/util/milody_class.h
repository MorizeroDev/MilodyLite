#ifndef MILODY_CLASS_H
#define MILODY_CLASS_H

#define MILODY_DECLARE_NON_COPYABLE(className)                                                                         \
    className(const className&) = delete;                                                                              \
    className& operator=(const className&) = delete;

#endif //MILODY_MILODY_CLASS_H
