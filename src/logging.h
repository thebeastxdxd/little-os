#ifndef __LOGGING_H__
#define __LOGGING_H__

#include "printf.h"
#include "vga.h"

// __VA_OPT__ is used here to allow the macro to not get any parameters except format
// __VA_ARGS__ is a macro to open up the arguments gotten
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
