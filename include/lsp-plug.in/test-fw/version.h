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
#define LSP_TEST_FW_MICRO           11

#ifdef LSP_TEST_FW_BUILTIN
    #define LSP_TEST_FW_EXPORT
    #define LSP_TEST_FW_CEXPORT
    #define LSP_TEST_FW_IMPORT          LSP_SYMBOL_IMPORT
    #define LSP_TEST_FW_CIMPORT         LSP_CSYMBOL_IMPORT
#else
    #define LSP_TEST_FW_EXPORT          LSP_SYMBOL_EXPORT
    #define LSP_TEST_FW_CEXPORT         LSP_CSYMBOL_EXPORT
    #define LSP_TEST_FW_IMPORT          LSP_SYMBOL_IMPORT
    #define LSP_TEST_FW_CIMPORT         LSP_CSYMBOL_IMPORT
#endif

#endif /* LSP_PLUG_IN_TEST_FW_VERSION_H_ */
