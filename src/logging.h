#ifndef __LOGGING_H__
#define __LOGGING_H__

#include "printf.h"
#include "vga.h"

#define LOG(format, ...)                                                       \
    printf("%s:%d:%s():" format "\n ", __FILE__, (int)__LINE__, __func__ __VA_OPT__(,)      \
           __VA_ARGS__)

#if defined(ENABLE_KERNEL_DEBUG)

#define DEBUG(format, ...) LOG(format __VA_OPT__(,) __VA_ARGS__)
#else
#define DEBUG(format, ...)                                                     \
    do {                                                                       \
    } while (0)

#endif

#endif
