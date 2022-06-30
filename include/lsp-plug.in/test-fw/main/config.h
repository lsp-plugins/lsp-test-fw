/*
 * Copyright (C) 2020 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2020 Vladimir Sadovnikov <sadko4u@gmail.com>
 *
 * This file is part of lsp-test-fw
 * Created on: 11 февр. 2019 г.
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

#ifndef LSP_TEST_FW_MAIN_CONFIG_H_
#define LSP_TEST_FW_MAIN_CONFIG_H_

#include <lsp-plug.in/test-fw/version.h>

#include <lsp-plug.in/test-fw/main/dynarray.h>
#include <lsp-plug.in/test-fw/main/status.h>
#include <lsp-plug.in/test-fw/types.h>

#include <stdio.h>

namespace lsp
{
    namespace test
    {
        enum test_mode_t
        {
            UNKNOWN,
            PTEST,
            UTEST,
            MTEST
        };

        typedef struct config_t
        {
            private:
                config_t & operator = (const config_t &);
                config_t(const config_t &);

            public:
                test_mode_t                 mode;
                bool                        fork;
                bool                        verbose;
                bool                        debug;
                bool                        list_all;
                bool                        mtrace;
                bool                        ilist;
                bool                        sysinfo;
                bool                        is_child;
                bool                        suppress;
                size_t                      threads;
                const char                 *executable;
                const char                 *outfile;
                char                       *tracepath;
                char                       *resource;       // Location of the resource directory
                char                       *tempdir;        // Location of the temporary directory
                dynarray_t                  list;
                dynarray_t                  ignore;
                dynarray_t                  args;
                FILE                       *std_out;        // Output stream
                FILE                       *std_err;        // Output error stream

            #ifdef LSP_TEST_FW_PLATFORM_WINDOWS
                size_t                      utf8_argc;
                char                      **utf8_argv;
            #endif /* LSP_TEST_FW_PLATFORM_WINDOWS */

            public:
                explicit config_t();
                ~config_t()         { clear(); }

                test_status_t       parse(FILE *out, int argc, const char **argv);
                test_status_t       print_usage(FILE *out, bool detailed=false);
                void                clear();

        } config_t;
    }
}

#endif /* LSP_TEST_FW_MAIN_CONFIG_H_ */
