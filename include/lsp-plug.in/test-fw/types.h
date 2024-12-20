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
#include <stdint.h>

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

#if defined(__WINDOWS__) || defined(__WIN32__) || defined(__WIN64__) || defined(_WIN64) || defined(_WIN32) || defined(__WINNT) || defined(__WINNT__)
    #define LSP_TEST_FW_PLATFORM_WINDOWS
    #define IF_LSP_TEST_FW_PLATFORM_WINDOWS(...)    __VA_ARGS__
#endif /* __WINDOWS__ */

#if defined(__HAIKU__)
    #define LSP_TEST_FW_PLATFORM_HAIKU
    #define IF_LSP_TEST_FW_PLATFORM_HAIKU(...)    __VA_ARGS__
#endif /* __Haiku__ */

#if defined(LSP_TEST_FW_PLATFORM_UNIX) || defined(LSP_TEST_FW_PLATFORM_LINUX) || defined(LSP_TEST_FW_PLATFORM_MACOSX) || defined(LSP_TEST_FW_PLATFORM_BSD) || defined(LSP_TEST_FW_PLATFORM_HAIKU)
    #define LSP_TEST_FW_PLATFORM_UNIX_COMPATIBLE
    #define LSP_TEST_FW_PLATFORM_POSIX

    #define IF_LSP_TEST_FW_PLATFORM_POSIX(...)      __VA_ARGS__
#endif /* unix-compatible platforms */

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

#ifndef IF_LSP_TEST_FW_PLATFORM_HAIKU
    #define IF_LSP_TEST_FW_PLATFORM_HAIKU(...)
#endif /* IF_LSP_TEST_FW_PLATFORM_HAIKU */

#ifndef IF_LSP_TEST_FW_PLATFORM_MACOSX
    #define IF_LSP_TEST_FW_PLATFORM_MACOSX(...)
#endif /* IF_LSP_TEST_FW_PLATFORM_MACOSX */

#ifndef IF_LSP_TEST_FW_PLATFORM_WINDOWS
    #define IF_LSP_TEST_FW_PLATFORM_WINDOWS(...)
#endif /* IF_LSP_TEST_FW_PLATFORM_WINDOWS */

//-----------------------------------------------------------------------------
// Detect endianess of architecture
#if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    #define LSP_TEST_FW_ARCH_LE
#elif (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    #define LSP_TEST_FW_ARCH_BE
#endif /* __BYTE_ORDER__ */

//-----------------------------------------------------------------------------
// File separator
#if defined(LSP_TEST_FW_PLATFORM_UNIX_COMPATIBLE)
    #define LSP_TEST_FW_FILE_SEPARATOR_C            '/'
    #define LSP_TEST_FW_FILE_SEPARATOR_S            "/"
#elif defined(LSP_TEST_FW_PLATFORM_WINDOWS)
    #define LSP_TEST_FW_FILE_SEPARATOR_C            '\\'
    #define LSP_TEST_FW_FILE_SEPARATOR_S            "\\"
#endif /* */

//-----------------------------------------------------------------------------
// Character settings
#if (WCHAR_MAX >= 0x10000ul)
    #define LSP_TEST_FW_WCHART_32BIT
#else
    #define LSP_TEST_FW_WCHART_16BIT
#endif /* WCHAR_MAX */

#define LSP_TEST_FW_DEFAULT_ALIGN       16

namespace lsp
{
    namespace test
    {
        // Wide size types
        typedef uint64_t        wsize_t;
        typedef int64_t         wssize_t;

        // Character type definition
    #if defined(LSP_TEST_FW_WCHART_16BIT)
        typedef wchar_t             utf16_t;
        typedef uint32_t            utf32_t;
    #else
        typedef uint16_t            utf16_t;
        typedef wchar_t             utf32_t;
    #endif

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
