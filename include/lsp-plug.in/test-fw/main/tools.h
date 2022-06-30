/*
 * Copyright (C) 2020 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2020 Vladimir Sadovnikov <sadko4u@gmail.com>
 *
 * This file is part of lsp-test-fw
 * Created on: 29 мар. 2020 г.
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

#ifndef LSP_PLUG_IN_TEST_FW_MAIN_TOOLS_H_
#define LSP_PLUG_IN_TEST_FW_MAIN_TOOLS_H_

#include <lsp-plug.in/test-fw/version.h>

#include <lsp-plug.in/test-fw/main/status.h>
#include <lsp-plug.in/test-fw/types.h>

#include <stdarg.h>

namespace lsp
{
    namespace test
    {
    #ifdef LSP_TEST_FW_PLATFORM_WINDOWS
        /** Append the Windows command line with additional text parameter
         *
         * @param buffer UTF-8 buffer that contains current command line
         * @param length length of the buffer
         * @param capacity capacity of the buffer
         * @param text text to append
         * @param space emit additional space before appending
         * @return status of operation
         */
        test_status_t cmdline_append_escaped(char **buffer, size_t *length, size_t *capacity, const char *text, bool space=true);

        /**
         * Convert UTF-8 string to UTF-16 string
         * @param str UTF-8 NULL-terminated string to convert
         * @return allocated UTF-16 string which should be free()'d after use or
         *   NULL if there is no sufficient memory
         */
        utf16_t *utf8_to_utf16(const char *str);

        /**
         * Convert UTF-16 string to UTF-8 string
         * @param str UTF-16 NULL-terminated string to convert
         * @return allocated UTF-8 string which should be free()'d after use or
         *   NULL if there is no sufficient memory
         */
        char *utf16_to_utf8(const utf16_t *str);

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
    #endif /* LSP_TEST_FW_PLATFORM_WINDOWS */

        /**
         * Recursively create directory
         * @param path UTF-8 path to the directory
         * @return status of operation
         */
        test_status_t mkdirs(const char *path);

        /**
         * Get default temporary directory
         * @return pointer to temporary directory which should be free()'d or NULL on error
         */
        char *get_default_temporary_path();

        /**
         * Get default resource directory
         * @return pointer to resource directory which should be free()'d or NULL on error
         */
        char *get_default_resource_path();

        /**
         * Get default trace directory
         * @return pointer to trace directory which should be free()'d or NULL on error
         */
        char *get_default_trace_path();
    }
}


#endif /* LSP_PLUG_IN_TEST_FW_MAIN_TOOLS_H_ */
