/*
 * version.h
 *
 *  Created on: 3 мая 2020 г.
 *      Author: sadko
 */

#ifndef LSP_PLUG_IN_TEST_FW_VERSION_H_
#define LSP_PLUG_IN_TEST_FW_VERSION_H_

// Define header version
#define LSP_TEST_FW_MAJOR           1
#define LSP_TEST_FW_MINOR           0
#define LSP_TEST_FW_MICRO           6

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
