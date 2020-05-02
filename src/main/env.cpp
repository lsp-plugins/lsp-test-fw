/*
 * env.cpp
 *
 *  Created on: 22 мар. 2020 г.
 *      Author: sadko
 */

#include <stdio.h>
#include <stdarg.h>

#include <lsp-plug.in/test-fw/env.h>

namespace lsp
{
    namespace test
    {
        Environment::Environment()
        {
            __verbose       = false;
            __executable    = NULL;
            __tempdir       = NULL;
            __resources     = NULL;
            __stdout        = NULL;
            __stderr        = NULL;
        }

        Environment::~Environment()
        {
            __verbose       = false;
            __executable    = NULL;
            __tempdir       = NULL;
            __resources     = NULL;
            __stdout        = NULL;
            __stderr        = NULL;
        }

        void Environment::configure(config_t *cfg)
        {
            __verbose       = cfg->verbose;
            __executable    = cfg->executable;
            __tempdir       = cfg->tempdir;
            __resources     = cfg->resource;
            __stdout        = cfg->std_out;
            __stderr        = cfg->std_err;
        }

        int Environment::printf(const char *fmt, ...)
        {
            if ((!__verbose) || (__stdout == NULL))
                return 0;

            va_list vl;
            va_start(vl, fmt);
            int res = ::vfprintf(__stdout, fmt, vl);
            va_end(vl);
            ::fflush(__stdout);
            return res;
        }

        int Environment::eprintf(const char *fmt, ...)
        {
            if (__stderr == NULL)
                return 0;

            va_list vl;
            va_start(vl, fmt);
            int res = ::vfprintf(__stderr, fmt, vl);
            va_end(vl);
            ::fflush(__stderr);
            return res;
        }
    }
}


