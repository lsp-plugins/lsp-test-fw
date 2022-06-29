/*
 * Copyright (C) 2020 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2020 Vladimir Sadovnikov <sadko4u@gmail.com>
 *
 * This file is part of lsp-test-fw
 * Created on: 21 мар. 2020 г.
 *
 * lsp-test-fw is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * lsp-test-fw is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with lsp-test-fw. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef LSP_PLUG_IN_TEST_FW_TYPES_H_
#define LSP_PLUG_IN_TEST_FW_TYPES_H_

#include <lsp-plug.in/test-fw/version.h>
#include <stddef.h>
#include <unistd.h>

//-----------------------------------------------------------------------------
// Detect build platform (part 1)
#if defined(__unix__) || defined(unix) || defined(__unix)
    #define LSP_TEST_FW_PLATFORM_UNIX
    #define IF_LSP_TEST_FW_PLATFORM_UNIX(...)       __VA_ARGS__
#endif /* __unix__ */

#if defined(__sun__) || defined(__sun) || defined(sun)
    #define LSP_TEST_FW_PLATFORM_SOLARIS
    #define IF_LSP_TEST_FW_PLATFORM_SOLARIS(...)    __VA_ARGS__
#endif /* __sun__ */

#if defined(__linux__) || defined(__linux) || defined(linux)
    #define LSP_TEST_FW_PLATFORM_LINUX
    #define IF_LSP_TEST_FW_PLATFORM_LINUX(...)      __VA_ARGS__
#endif /* __linux__ */

#if defined(__FreeBSD__)
    #define LSP_TEST_FW_PLATFORM_FREEBSD
    #define IF_LSP_TEST_FW_PLATFORM_FREEBSD(...)    __VA_ARGS__
#endif /* __FreeBSD__ */

#if defined(__OpenBSD__)
    #define LSP_TEST_FW_PLATFORM_OPENBSD
    #define IF_LSP_TEST_FW_PLATFORM_OPENBSD(...)    __VA_ARGS__
#endif /* __FreeBSD__ */

#if defined(__bsd__) || defined(__bsd) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(freebsd) || defined(openbsd) || defined(bsdi) || defined(__darwin__)
    #define LSP_TEST_FW_PLATFORM_BSD
    #define IF_LSP_TEST_FW_PLATFORM_BSD(...)        __VA_ARGS__
#endif /* __bsd__ */

#if defined(__macosx__) || defined(__APPLE__) || defined(__MACH__) || defined(__DARWIN__)
    #define LSP_TEST_FW_PLATFORM_MACOSX
    #define IF_LSP_TEST_FW_PLATFORM_MACOSX(...)     __VA_ARGS__
#endif /* __macosx__ */

#if defined(LSP_TEST_FW_PLATFORM_UNIX) || defined(LSP_TEST_FW_PLATFORM_LINUX) || defined(LSP_TEST_FW_PLATFORM_MACOSX) || defined(LSP_TEST_FW_PLATFORM_BSD)
    #define LSP_TEST_FW_PLATFORM_UNIX_COMPATIBLE
    #define LSP_TEST_FW_PLATFORM_POSIX

    #define IF_LSP_TEST_FW_PLATFORM_POSIX(...)      __VA_ARGS__
#endif /* unix-compatible platforms */

#if defined(__WINDOWS__) || defined(__WIN32__) || defined(__WIN64__) || defined(_WIN64) || defined(_WIN32) || defined(__WINNT) || defined(__WINNT__)
    #define LSP_TEST_FW_PLATFORM_WINDOWS
    #define IF_LSP_TEST_FW_PLATFORM_WINDOWS(...)    __VA_ARGS__
#endif /* __WINDOWS__ */

//-----------------------------------------------------------------------------
// Detect build platform (part 2)
#ifndef IF_LSP_TEST_FW_PLATFORM_UNIX
    #define IF_LSP_TEST_FW_PLATFORM_UNIX(...)
#endif /* IF_LSP_TEST_FW_PLATFORM_UNIX */

#ifndef IF_LSP_TEST_FW_PLATFORM_SOLARIS
    #define IF_LSP_TEST_FW_PLATFORM_SOLARIS(...)
#endif /* IF_LSP_TEST_FW_PLATFORM_SOLARIS */

#ifndef IF_LSP_TEST_FW_PLATFORM_LINUX
    #define IF_LSP_TEST_FW_PLATFORM_LINUX(...)
#endif /* IF_LSP_TEST_FW_PLATFORM_LINUX */

#ifndef IF_LSP_TEST_FW_PLATFORM_POSIX
    #define IF_LSP_TEST_FW_PLATFORM_POSIX(...)
#endif /* IF_LSP_TEST_FW_PLATFORM_POSIX */

#ifndef IF_LSP_TEST_FW_PLATFORM_BSD
    #define IF_LSP_TEST_FW_PLATFORM_BSD(...)
#endif /* IF_LSP_TEST_FW_PLATFORM_BSD */

#ifndef IF_LSP_TEST_FW_PLATFORM_MACOSX
    #define IF_LSP_TEST_FW_PLATFORM_MACOSX(...)
#endif /* IF_LSP_TEST_FW_PLATFORM_MACOSX */

#ifndef IF_LSP_TEST_FW_PLATFORM_WINDOWS
    #define IF_LSP_TEST_FW_PLATFORM_WINDOWS(...)
#endif /* IF_LSP_TEST_FW_PLATFORM_WINDOWS */


//-----------------------------------------------------------------------------
// Configure the export symbol
#ifdef LSP_TEST_FW_BUILTIN
    #define LSP_TEST_FW_EXPORT
#else
    #ifdef LSP_TEST_FW_PLATFORM_WINDOWS
        #define LSP_TEST_FW_EXPORT              __declspec(dllexport)
    #else
        #define LSP_TEST_FW_EXPORT              __attribute__((visibility("default")))
    #endif /* LSP_TEST_FW_PLATFORM_WINDOWS */
#endif /* LSP_TEST_FW_BUILTIN */

#define LSP_TEST_FW_DEFAULT_ALIGN       16

namespace lsp
{
    namespace test
    {
        // Align pointer to the specified boundary
        template <class T>
            inline T *align_pointer(void *src, size_t align)
            {
                ptrdiff_t x     = ptrdiff_t(src);
                ptrdiff_t off   = (x % align);

                return (off == 0) ?
                        reinterpret_cast<T *>(src) :
                        reinterpret_cast<T *>(x + align - off);
            }

        // Check that pointer is aligned to the specified boundary
        template <class T>
            inline bool check_alignment(const T *src, size_t align)
            {
                ptrdiff_t x     = ptrdiff_t(src);
                return !(x % align);
            }
    }
}

#endif /* LSP_PLUG_IN_TEST_FW_TYPES_H_ */
