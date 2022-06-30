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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <lsp-plug.in/test-fw/main/tools.h>

#ifdef LSP_TEST_FW_PLATFORM_WINDOWS
    #include <shlwapi.h>
    #include <fileapi.h>
#else
    #include <sys/stat.h>
#endif /* LSP_TEST_FW_PLATFORM_WINDOWS */

namespace lsp
{
    namespace test
    {
    #ifdef LSP_TEST_FW_PLATFORM_WINDOWS
        static test_status_t cmdline_append_char(char **buffer, size_t *length, size_t *capacity, char ch)
        {
            char *dst           = *buffer;
            if (*length < *capacity)
            {
                dst[(*length)++]    = ch;
                return LSP_TEST_FW_OK;
            }

            *capacity   = (*capacity > 0) ? ((*capacity) << 1) : 32;
            dst         = static_cast<char *>(::realloc(dst, *capacity + 1)); // Do not count the last '\0' character as capacity
            if (dst == NULL)
                return LSP_TEST_FW_NO_MEM;
            *buffer     = dst;
            dst[(*length)++]    = ch;

            return LSP_TEST_FW_OK;
        }

        test_status_t cmdline_append_escaped(char **buffer, size_t *length, size_t *capacity, const char *text, bool space)
        {
            char ch;
            test_status_t res   = LSP_TEST_FW_OK;

            // Append space if needed
            if (space)
            {
                res = cmdline_append_char(buffer, length, capacity, ' ');
                if (res != LSP_TEST_FW_OK)
                    return res;
            }

            // Check argument length
            if (*text == '\0')
            {
                // Empty argument
                res = cmdline_append_char(buffer, length, capacity, '\"');
                if (res == LSP_TEST_FW_OK)
                    res = cmdline_append_char(buffer, length, capacity, '\"');
            }
            else
            {
                // Non-empty argument, process it:
                //   ' ' -> '\"', ' ', '\"'
                //   '"' -> '\\', '\"'
                while ((ch = *(text++)) != '\0')
                {
                    switch (ch)
                    {
                        case ' ':
                            res = cmdline_append_char(buffer, length, capacity, '\"');
                            if (res == LSP_TEST_FW_OK)
                                res = cmdline_append_char(buffer, length, capacity, ' ');
                            if (res == LSP_TEST_FW_OK)
                                res = cmdline_append_char(buffer, length, capacity, '\"');
                            break;
                        case '"':
                            res = cmdline_append_char(buffer, length, capacity, '\\');
                            if (res == LSP_TEST_FW_OK)
                                res = cmdline_append_char(buffer, length, capacity, '\"');
                            break;
                        default:
                            res = cmdline_append_char(buffer, length, capacity, ch);
                            break;
                    }

                    if (res != LSP_TEST_FW_OK)
                        break;
                }
            }

            // Store end-of-string character
            (*buffer)[*length]  = '\0';

            return res;
        }

        static inline size_t sizeof_utf16(utf32_t cp)
        {
            return (cp < 0x10000) ? 2 : 4;
        }

        static inline size_t sizeof_utf8(utf32_t cp)
        {
            if (cp >= 0x800)
                return ((cp < 0x10000) || (cp >= 0x200000)) ? 3 : 4;
            else
                return (cp >= 0x80) ? 2 : 1;
        }

        static void write_utf16_codepoint(utf16_t **str, utf32_t cp)
        {
            uint8_t *dst        = reinterpret_cast<uint8_t *>(*str);
            if (cp < 0x10000)
            {
                #ifdef LSP_TEST_FW_ARCH_LE
                    dst[0]          = uint8_t(cp & 0xff);
                    dst[1]          = uint8_t(cp >> 8);
                #else
                    dst[0]          = uint8_t(cp >> 8);
                    dst[1]          = uint8_t(cp & 0xff);
                #endif
                *str           += 1;
            }
            else
            {
                cp             -= 0x10000;
                utf16_t w1      = utf16_t(0xd800 | (cp >> 10));
                utf16_t w2      = utf16_t(0xdc00 | (cp & 0x3ff));

                #ifdef LSP_TEST_FW_ARCH_LE
                    dst[0]          = uint8_t(w1 & 0xff);
                    dst[1]          = uint8_t(w1 >> 8);
                    dst[2]          = uint8_t(w2 & 0xff);
                    dst[3]          = uint8_t(w2 >> 8);
                #else
                    dst[0]          = uint8_t(w1 >> 8);
                    dst[1]          = uint8_t(w1 & 0xff);
                    dst[2]          = uint8_t(w2 >> 8);
                    dst[3]          = uint8_t(w2 & 0xff);
                #endif
                *str           += 2;
            }
        }

        void write_utf8_codepoint(char **str, utf32_t cp)
        {
            char *dst = *str;
            if (cp >= 0x800) // 3-4 bytes
            {
                if (cp < 0x10000) // 3 bytes
                {
                    dst[0]      = (cp >> 12) | 0xe0;
                    dst[1]      = ((cp >> 6) & 0x3f) | 0x80;
                    dst[2]      = (cp & 0x3f) | 0x80;
                    dst        += 3;
                }
                else if (cp < 0x200000) // 4 bytes
                {
                    dst[0]      = (cp >> 16) | 0xf0;
                    dst[1]      = ((cp >> 12) & 0x3f) | 0x80;
                    dst[2]      = ((cp >> 6) & 0x3f) | 0x80;
                    dst[3]      = (cp & 0x3f) | 0x80;
                    dst        += 4;
                }
                else // Invalid character, emit 3 bytes of 0xfffd code point value
                {
                    dst[0]      = 0xef;
                    dst[1]      = 0xbf;
                    dst[2]      = 0xbd;
                    dst        += 3;
                }
            }
            else // 1-2 bytes
            {
                if (cp >= 0x80) // 2 bytes
                {
                    dst[0]      = (cp >> 6) | 0xc0;
                    dst[1]      = (cp & 0x3f) | 0x80;
                    dst        += 2;
                }
                else // 1 byte
                    *(dst++)    = char(cp);
            }
            *str    = dst;
        }

        static utf32_t read_utf8_codepoint(const char **str)
        {
            utf32_t cp, sp;
            size_t bytes;
            const char *s = *str;

            // Decode primary byte
            cp = uint8_t(*s);
            if (cp <= 0x7f)
            {
                *str    = (cp == 0) ? s : s+1;
                return cp;
            }

            ++s;
            if ((cp & 0xe0) == 0xc0) // 2 bytes: 110xxxxx 10xxxxxx
            {
                cp     &= 0x1f;
                bytes   = (cp >= 0x02) ? 1 : 0;
            }
            else if ((cp & 0xf0) == 0xe0) // 3 bytes: 1110xxxx 10xxxxxx 10xxxxxx
            {
                cp     &= 0x0f;
                bytes   = (cp) ? 2 : 0;
            }
            else if ((cp & 0xf8) == 0xf0) // 4 bytes: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
            {
                cp     &= 0x07;
                bytes   = 3;
            }
            else
                bytes   = 0;

            // Invalid first byte sequence?
            if (!bytes)
            {
                *str    = s;
                return 0xfffd;
            }

            // Decode extension bytes
            for (size_t i=0; i<bytes; ++i)
            {
                sp  = uint8_t(*s);
                if ((sp & 0xc0) != 0x80) // Invalid sequence?
                {
                    *str    = (sp == 0) ? s : s+1;
                    return 0xfffd;
                }
                cp     = (cp << 6) | (sp & 0x3f);
                ++s;
            }

            if ((bytes == 3) && (cp < 0x10000)) // Check that 4-byte sequence is valid
                cp      = 0xfffd;
            else if ((cp >= 0xd800) && (cp < 0xe000)) // Check for surrogates
                cp      = 0xfffd;

            *str = s;
            return cp;
        }

        static utf32_t read_utf16_codepoint(const utf16_t **str)
        {
            uint32_t cp, sc;
            const uint8_t *s = reinterpret_cast<const uint8_t *>(*str);

            #ifdef LSP_TEST_FW_ARCH_LE
                cp = s[0] | (s[1] << 8);
            #else
                cp = (s[0] << 8) | s[1];
            #endif
            s += 2;
            if (cp == 0)
            {
                *str += 1;
                return cp;
            }

            sc = cp & 0xfc00;
            if (sc == 0xd800) // cp = Surrogate high
            {
                #ifdef LSP_TEST_FW_ARCH_LE
                    sc = s[0] | (s[1] << 8);
                #else
                    sc = (s[0] << 8) | s[1];
                #endif
                if ((sc & 0xfc00) == 0xdc00)
                {
                    *str += 2;
                    cp  = 0x10000 | ((cp & 0x3ff) << 10) | (sc & 0x3ff);
                }
                else
                    cp  = 0xfffd;
            }
            else if (sc == 0xdc00) // Surrogate low?
            {
                #ifdef LSP_TEST_FW_ARCH_LE
                    sc = s[0] | (s[1] << 8);
                #else
                    sc = (s[0] << 8) | s[1];
                #endif
                if ((sc & 0xfc00) == 0xd800)
                {
                    *str += 2;
                    cp  = 0x10000 | ((sc & 0x3ff) << 10) | (cp & 0x3ff);
                }
                else
                    cp  = 0xfffd;
            }
            else
                *str += 1;

            return cp;
        }

        utf16_t *utf8_to_utf16(const char *str)
        {
            // Estimate number of bytes to allocate
            utf32_t cp;
            size_t bytes    = 0;
            const char *p = str;
            do
            {
                cp      = read_utf8_codepoint(&p);
                bytes  += sizeof_utf16(cp);
            } while (cp != 0);

            // Allocate memory
            utf16_t *utf16  = static_cast<utf16_t *>(::malloc(bytes));
            if (utf16 == NULL)
                return NULL;

            // Perform encoding
            utf16_t *dst = utf16;
            p               = str;
            while ((cp = read_utf8_codepoint(&p)) != 0)
                write_utf16_codepoint(&dst, cp);
            *dst        = 0;

            return utf16;
        }

        char *utf16_to_utf8(const utf16_t *str)
        {
            // Estimate number of bytes
            utf32_t cp;
            size_t bytes = 0;
            const utf16_t *p = str;
            do
            {
                cp          = read_utf16_codepoint(&p);
                bytes      += sizeof_utf8(cp);
            } while (cp != 0);

            // Allocate memory
            char *utf8  = static_cast<char *>(::malloc(bytes));
            if (utf8 == NULL)
                return NULL;

            // Now perform encoding
            char *dst   = utf8;
            p           = str;
            while ((cp = read_utf16_codepoint(&p)) != 0)
                write_utf8_codepoint(&dst, cp);
            *dst = '\0';

            return utf8;
        }

        int vasprintf(char **res, const char *fmt, va_list ap)
        {
            int len = vsnprintf(NULL, 0, fmt, ap);
            if (len < 0)
                return -1;

            char *str = static_cast<char *>(malloc(size_t(len) + 1));
            if (str == NULL)
                return -1;

            int r = vsnprintf(str, len + 1, fmt, ap);
            if (r < 0)
            {
                free(str);
                return -1;
            }

            *res = str;
            return r;
        }

        int asprintf(char **strp, const char *fmt, ...)
        {
            va_list ap;
            va_start(ap, fmt);

            int r = vasprintf(strp, fmt, ap);

            va_end(ap);
            return r;
        }

        char *get_env_var(const char *name)
        {
            // Convert variable name to UTF-16
            utf16_t *varname = utf8_to_utf16(name);
            if (!varname)
                return NULL;

            // Estimate the required buffer size
            size_t size = ::GetEnvironmentVariableW(varname, NULL, 0);
            if (size == 0)
            {
                ::free(varname);
                return NULL;
            }

            // Allocate buffer
            utf16_t *buf = static_cast<utf16_t *>(::malloc(size * sizeof(utf16_t)));
            if (buf == NULL)
            {
                ::free(varname);
                return NULL;
            }

            // Get environment variable
            size = ::GetEnvironmentVariableW(varname, buf, size);
            ::free(varname);
            if (size == 0)
                return NULL;

            // Convert to UTF-8 and return
            char *res = utf16_to_utf8(buf);
            ::free(buf);
            return res;
        }

        char *get_temp_dir()
        {
            char *temp = get_env_var("TEMP");
            if (!temp)
                temp = get_env_var("TMP");
            return (temp) ? temp : ::strdup("tmp");
        }

        char *get_default_temporary_path()
        {
            char *temp = get_temp_dir();
            char *res = NULL;

            int n = asprintf(&res, "%s" LSP_TEST_FW_FILE_SEPARATOR_S "lsp-test-temp", temp);
            ::free(temp);

            if ((n < 0) && (res != NULL))
            {
                ::free(res);
                res = NULL;
            }

            return res;
        }

        char *get_default_trace_path()
        {
            char *temp = get_temp_dir();
            char *res = NULL;

            int n = asprintf(&res, "%s" LSP_TEST_FW_FILE_SEPARATOR_S "lsp-test-trace", temp);
            ::free(temp);

            if ((n < 0) && (res != NULL))
            {
                ::free(res);
                res = NULL;
            }

            return res;
        }

        test_status_t mkdirs(const char *path)
        {
            utf16_t *wpath = utf8_to_utf16(path);
            if (!wpath)
                return LSP_TEST_FW_NO_MEM;

            // Skip first separator if path is not relative
            utf16_t *curr = wpath;
            if (!::PathIsRelativeW(reinterpret_cast<LPCWSTR>(wpath)))
            {
                while ((*curr != 0) && (*curr != LSP_TEST_FW_FILE_SEPARATOR_C))
                    ++curr;
                if (*curr == 0)
                    return LSP_TEST_FW_OK;
                ++curr;
            }

            test_status_t res = LSP_TEST_FW_OK;

            while (true)
            {
                // Get next split character
                utf16_t *p = curr;
                while ((*p != 0) && (*p != LSP_TEST_FW_FILE_SEPARATOR_C))
                    ++p;
                if (*p == 0)
                    p   = NULL;
                else
                    *p  = 0;

                // Test for existence
                DWORD x = ::GetFileAttributesW(wpath);
                if (x == INVALID_FILE_ATTRIBUTES)
                {
                    // Try to create directory
                    if (!::CreateDirectoryW(wpath, NULL))
                    {
                        DWORD x = ::GetFileAttributesW(wpath);
                        if (x == INVALID_FILE_ATTRIBUTES)
                        {
                            res = LSP_TEST_FW_PERMISSION_DENIED;
                            break;
                        }
                    }
                }

                // Check the final result
                if (!(x & FILE_ATTRIBUTE_DIRECTORY))
                {
                    res = LSP_TEST_FW_NOT_DIRECTORY;
                    break;
                }

                // Recover the path, move iterator to the next pathname
                if (p == NULL) // Last path item?
                    break;
                *p   = LSP_TEST_FW_FILE_SEPARATOR_C;
                curr = p + 1;
            }

            // Free allocated resource
            ::free(wpath);

            return res;
        }

    #else
        test_status_t mkdirs(const char *path)
        {
            char *tmp = ::strdup(path);
            if (!tmp)
                return LSP_TEST_FW_NO_MEM;

            test_status_t res = LSP_TEST_FW_OK;

            // Skip first separator
            char *curr = tmp;
            if (*curr == LSP_TEST_FW_FILE_SEPARATOR_C)
                ++curr;

            struct stat fs;

            while (true)
            {
                // Get next split character
                char *p = ::strchr(curr, LSP_TEST_FW_FILE_SEPARATOR_C);
                if (p != NULL)
                    *p = '\0';

                // Test for existence
                int x = ::stat(tmp, &fs);
                if (x != 0)
                {
                    // Try to create directory
                    x = ::mkdir(tmp, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

                    // If failed - stat again (possible concurrency)
                    if (x != 0)
                        x = ::stat(tmp, &fs);
                }

                // Check the final result
                if (x != 0)
                {
                    res = LSP_TEST_FW_IO_ERROR;
                    break;
                }

                // Recover the path, move iterator to the next pathname
                if (p == NULL) // Last path item?
                    break;
                *p = LSP_TEST_FW_FILE_SEPARATOR_C;
                curr = p + 1;
            }

            // Free allocated resource
            ::free(tmp);

            return res;
        }

        char *get_default_temporary_path()
        {
            return ::strdup("/tmp/lsp-test-temp");
        }

        char *get_default_trace_path()
        {
            return ::strdup("/tmp/lsp-test-trace");
        }
    #endif /* LSP_TEST_FW_PLATFORM_WINDOWS */

        char *get_default_resource_path()
        {
            return ::strdup("res" LSP_TEST_FW_FILE_SEPARATOR_S "test");
        }
    }
}
