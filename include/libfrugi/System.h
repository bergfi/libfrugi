/*
 * System.h
 * 
 * Part of a general library.
 * 
 * @author Freark van der Berg
 */
#pragma once

#if __linux__
#include <sys/sysinfo.h>
#endif
#if __APPLE__
#include <unistd.h>
#endif

#include <cassert>
#include <ctime>
#include <string>
#include <random>
#include <vector>

#include <libfrugi/Config.h>
#include <libfrugi/FileSystem.h>

namespace libfrugi {

class System {
public:
    class Timer {
    private:
#if WIN32
        LARGE_INTEGER freq;
        LARGE_INTEGER start;
#else
        timespec start;
#endif
    public:
        Timer() {
            reset();
        }

        static Timer* create() {
            return new Timer();
        }

        void reset() {
#if LIBFRUGI_SYSTEM_TIMER_BACKEND == LIBFRUGI_SYSTEM_TIMER_BACKEND_WINDOWS
            QueryPerformanceFrequency(&freq);
            QueryPerformanceCounter(&start);
#endif
#if LIBFRUGI_SYSTEM_TIMER_BACKEND == LIBFRUGI_SYSTEM_TIMER_BACKEND_MONOTONIC || LIBFRUGI_SYSTEM_TIMER_BACKEND == LIBFRUGI_SYSTEM_TIMER_BACKEND_MONOTONIC_RAW
            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
#endif
        }

        double getElapsedSeconds() {
#if LIBFRUGI_SYSTEM_TIMER_BACKEND == LIBFRUGI_SYSTEM_TIMER_BACKEND_WINDOWS
            LARGE_INTEGER now;
            QueryPerformanceCounter(&now);
            return (double)(now.QuadPart-start.QuadPart) / (double)(freq.QuadPart) ;
#endif
#if LIBFRUGI_SYSTEM_TIMER_BACKEND == LIBFRUGI_SYSTEM_TIMER_BACKEND_MONOTONIC || LIBFRUGI_SYSTEM_TIMER_BACKEND == LIBFRUGI_SYSTEM_TIMER_BACKEND_MONOTONIC_RAW
            timespec now;
            clock_gettime(CLOCK_MONOTONIC_RAW, &now);
            return (double) (now.tv_sec - start.tv_sec)
                   + (double) (now.tv_nsec - start.tv_nsec) * 0.000000001;
#endif
        }
    };

    static void init(int argc, char* argv[]);

    static void sleep(uint64_t ms);

    static void generateUUID(size_t bytes, std::string& uuid);

    static uint64_t getCurrentTimeMillis();

    static uint64_t getCurrentTimeMicros();

    static std::string getBinaryLocation() {
        return cwdAtStart;
    }

    static std::string const& getArgument(size_t i);

    static std::vector<std::string> const& getArguments();

    static size_t getNumberOfAvailableCores() {
#if defined(__linux__)
        int cpu_count = sysconf(_SC_NPROCESSORS_ONLN);
        return cpu_count;
#elif defined(__APPLE__)
        int cpu_count = sysconf(_SC_NPROCESSORS_ONLN);
        return cpu_count;
#elif defined(_WIN32)
        SYSTEM_INFO siSysInfo;
        GetSystemInfo(&siSysInfo);
        return siSysInfo.dwNumberOfProcessors;
#else
#   error Building for unsupported system
#endif
    }

private:
    static std::string cwdAtStart;
    static std::vector<std::string> args;
};

} // namespace libfrugi