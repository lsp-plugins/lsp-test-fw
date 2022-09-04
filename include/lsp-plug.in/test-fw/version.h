/*
 * Copyright (C) 2020 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2020 Vladimir Sadovnikov <sadko4u@gmail.com>
 *
 * This file is part of lsp-test-fw
 * Created on: 3 мая 2020 г.
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

#ifndef LSP_PLUG_IN_TEST_FW_VERSION_H_
#define LSP_PLUG_IN_TEST_FW_VERSION_H_

// Define header version
#define LSP_TEST_FW_MAJOR           1
#define LSP_TEST_FW_MINOR           0
#define LSP_TEST_FW_MICRO           16

#if defined(__WINDOWS__) || defined(__WIN32__) || defined(__WIN64__) || defined(_WIN64) || defined(_WIN32) || defined(__WINNT) || defined(__WINNT__)
    #define LSP_TEST_FW_EXPORT_MODIFIER     __declspec(dllexport)
    #define LSP_TEST_FW_IMPORT_MODIFIER     __declspec(dllimport)
#else
    #define LSP_TEST_FW_EXPORT_MODIFIER     __attribute__((visibility("default")))
    #define LSP_TEST_FW_IMPORT_MODIFIER
#endif /* __WINDOWS__ */

#if defined(LSP_TEST_FW_PUBLISHER)
    #define LSP_TEST_FW_PUBLIC          LSP_TEST_FW_EXPORT_MODIFIER
#elif defined(LSP_TEST_FW_BUILTIN) || defined(LSP_IDE_DEBUG)
    #define LSP_TEST_FW_PUBLIC
#else
    #define LSP_TEST_FW_PUBLIC          LSP_TEST_FW_IMPORT_MODIFIER
#endif

#endif /* LSP_PLUG_IN_TEST_FW_VERSION_H_ */
