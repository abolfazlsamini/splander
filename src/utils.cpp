#include <cstdint>
#include "utils.h"
#ifdef WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#include <ctime>
#endif

/* Returns the amount of milliseconds elapsed since the UNIX epoch. Works on both
 * windows and linux. */
int64_t GetTimeMs64()
{
#ifdef WIN32
    FILETIME ft;
    LARGE_INTEGER li;

    GetSystemTimeAsFileTime(&ft);
    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;

    uint64 ret = li.QuadPart;
    ret -= 116444736000000000LL;
    ret /= 10000;

    return ret;
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    uint64_t ret = tv.tv_usec;
    ret /= 1000;
    ret += (tv.tv_sec * 1000);
    return ret;
#endif
}

// struct timeval milli_time;
// double millisec = 0;
// time_t currentTime;
// SYSTEMTIME st;
// unsigned long long SystemTimeToEpochMillis(const SYSTEMTIME& st) {
//     FILETIME ft;
//     SystemTimeToFileTime(&st, &ft);
//
//     // Convert FILETIME to ULARGE_INTEGER
//     ULARGE_INTEGER ull;
//     ull.LowPart = ft.dwLowDateTime;
//     ull.HighPart = ft.dwHighDateTime;
//
//     const unsigned long long EPOCH_OFFSET = 116444736000000000ULL;
//     return (ull.QuadPart - EPOCH_OFFSET) / 10000ULL;
// }//TODO not sure if this gives the same time as the linux in windows is something like this: 1739270417128
