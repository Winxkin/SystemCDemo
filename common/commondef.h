#ifndef _COMMONDEF_H
#define _COMMONDEF_H
#include <iostream>
#include <cstdio>
#include <cstdarg>
#include <systemc>

// Define the LOG macro to print timestamp in nanoseconds
#define LOG(fmt, ...) \
    do { \
        int64_t ns = static_cast<int64_t>(sc_core::sc_time_stamp().to_double() / 1000); \
        printf("[%ld NS ] " fmt, ns, ##__VA_ARGS__); \
    } while (0)

#endif