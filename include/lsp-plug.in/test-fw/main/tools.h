/*
 * tools.h
 *
 *  Created on: 29 мар. 2020 г.
 *      Author: sadko
 */

#ifndef LSP_PLUG_IN_TEST_FW_MAIN_TOOLS_H_
#define LSP_PLUG_IN_TEST_FW_MAIN_TOOLS_H_

#include <stdarg.h>

#include <lsp-plug.in/common/types.h>
#include <lsp-plug.in/common/status.h>

namespace lsp
{
    namespace test
    {
#ifdef PLATFORM_WINDOWS
        /** Append the Windows command line with additional text parameter
         *
         * @param buffer UTF-8 buffer that contains current command line
         * @param length length of the buffer
         * @param capacity capacity of the buffer
         * @param text text to append
         * @param space emit additional space before appending
         * @return status of operation
         */
        status_t cmdline_append_escaped(char **buffer, size_t *length, size_t *capacity, const char *text, bool space=true);

        /**
         * Convert UTF-8 string to UTF-16 string
         * @param str UTF-8 NULL-terminated string to convert
         * @return allocated UTF-16 string which should be free()'d after use or
         *   NULL if there is no sufficient memory
         */
        lsp_utf16_t *utf8_to_utf16(const char *str);

        /**
         * Convert UTF-16 string to UTF-8 string
         * @param str UTF-16 NULL-terminated string to convert
         * @return allocated UTF-8 string which should be free()'d after use or
         *   NULL if there is no sufficient memory
         */
        char *utf16_to_utf8(const lsp_utf16_t *str);

        /**
         * There's no vasprintf for Windows, implement it
         * @param strp pointer to store string pointer
         * @param fmt format
         * @param ap argument list
         * @return number of printed bytes
         */
        int vasprintf(char **strp, const char *fmt, va_list ap);

        /**
         * There's no asprintf for Windows, implement it
         * @param strp pointer to store string pointer
         * @param fmt format
         * @return number of printed bytes
         */
        int asprintf(char **strp, const char *fmt, ...);
#endif
    }
}


#endif /* LSP_PLUG_IN_TEST_FW_MAIN_TOOLS_H_ */
