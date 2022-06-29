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

#if defined(__WINDOWS__) || defined(__WIN32__) || defined(__WIN64__) || defined(_WIN64) || defined(_WIN32) || defined(__WINNT) || defined(__WINNT__)
    #define LSP_TEST_FW_PLATFORM_WINDOWS
#endif /* WINDOWS */

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

        template <class T>
            inline T *align_pointer(void *src, size_t align)
            {
                ptrdiff_t x     = ptrdiff_t(src);
                ptrdiff_t off   = (x % align);

                return (off == 0) ?
                        reinterpret_cast<T *>(src) :
                        reinterpret_cast<T *>(x + align - off);
            }

        template <class T>
            inline bool check_alignment(const T *src, size_t align)
            {
                ptrdiff_t x     = ptrdiff_t(src);
                return !(x % align);
            }
    }
}

#endif /* LSP_PLUG_IN_TEST_FW_TYPES_H_ */
