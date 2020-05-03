/*
 * types.h
 *
 *  Created on: 11 февр. 2019 г.
 *      Author: sadko
 */

#ifndef TEST_MAIN_TYPES_H_
#define TEST_MAIN_TYPES_H_

#include <lsp-plug.in/test-fw/version.h>
#include <lsp-plug.in/test-fw/types.h>
#include <lsp-plug.in/test-fw/main/dynarray.h>

#include <sys/stat.h>
#include <time.h>

#ifdef PLATFORM_WINDOWS
    #include <processthreadsapi.h>
    #include <sysinfoapi.h>
    #include <errhandlingapi.h>
#endif

#ifdef PLATFORM_UNIX_COMPATIBLE
    #include <unistd.h>
    #include <sys/wait.h>

    #include <fcntl.h>
#endif /* PLATFORM_UNIX_COMPATIBLE */

#ifdef PLATFORM_LINUX
    #include <mcheck.h>
#endif /* PLATFORM_LINUX */

namespace lsp
{
    namespace test
    {
#if defined(PLATFORM_WINDOWS)
        typedef PROCESS_INFORMATION     test_pid_t;
        typedef FILETIME                test_clock_t;
#else
        typedef pid_t                   test_pid_t;
        typedef struct timespec         test_clock_t;
#endif

        class UnitTest;

        typedef struct stats_t
        {
            size_t              total;
            double              overall;

            dynarray_t          success;        // List of failed tests
            dynarray_t          failed;         // List of failed tests
            dynarray_t          ignored;        // List of ignored tests
        } stats_t;

        typedef struct task_t
        {
            pid_t               pid;
            struct timespec     submitted;
            test::UnitTest     *utest;
        } task_t;

    #if defined(PLATFORM_WINDOWS)
        inline void get_test_time(test_clock_t *clock)
        {
            GetSystemTimeAsFileTime(clock);
        }

        inline double calc_test_time_difference(const test_clock_t *begin, const test_clock_t *end)
        {
            uint64_t ibegin = (uint64_t(begin->dwHighDateTime) << 32) | begin->dwLowDateTime;
            uint64_t iend   = (uint64_t(end->dwHighDateTime) << 32) | end->dwLowDateTime;
            return (iend - ibegin) * 1e-7;
        }
    #else
        inline void get_test_time(test_clock_t *clock)
        {
            clock_gettime(CLOCK_REALTIME, clock);
        }

        inline double calc_test_time_difference(const test_clock_t *begin, const test_clock_t *end)
        {
            return (end->tv_sec - begin->tv_sec) + (end->tv_nsec - begin->tv_nsec) * 1e-9;
        }
    #endif /* PLATFORM_WINDOWS */
    }
}

#endif /* TEST_MAIN_TYPES_H_ */
