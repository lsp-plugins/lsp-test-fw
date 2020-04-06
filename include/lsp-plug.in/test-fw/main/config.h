/*
 * config.h
 *
 *  Created on: 11 февр. 2019 г.
 *      Author: sadko
 */

#ifndef TEST_MAIN_CONFIG_H_
#define TEST_MAIN_CONFIG_H_

#include <stdio.h>

#include <lsp-plug.in/common/status.h>

#include <lsp-plug.in/test-fw/types.h>
#include <lsp-plug.in/test-fw/main/dynarray.h>

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
                size_t                      threads;
                const char                 *executable;
                const char                 *outfile;
                const char                 *tracepath;
                dynarray_t                  list;
                dynarray_t                  ignore;
                dynarray_t                  args;
                FILE                       *std_out;        // Output stream
                FILE                       *std_err;        // Output error stream

    #ifdef PLATFORM_WINDOWS
                size_t                      utf8_argc;
                char                      **utf8_argv;
    #endif /* PLATFORM_WINDOWS */

            public:
                explicit config_t();
                ~config_t()         { clear(); }

                status_t        parse(FILE *out, int argc, const char **argv);
                status_t        print_usage(FILE *out, bool detailed=false);
                void            clear();

        } config_t;
    }
}

#endif /* TEST_MAIN_CONFIG_H_ */
