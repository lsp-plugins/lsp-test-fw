/*
 * Copyright (C) 2020 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2020 Vladimir Sadovnikov <sadko4u@gmail.com>
 *
 * This file is part of lsp-test-fw
 * Created on: 22 мар. 2020 г.
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

#include <errno.h>
#include <string.h>

#include <lsp-plug.in/test-fw/main/config.h>
#include <lsp-plug.in/test-fw/main/tools.h>

#ifdef LSP_TEST_FW_PLATFORM_WINDOWS
    #include <windows.h>
#endif /* LSP_TEST_FW_PLATFORM_WINDOWS */

namespace lsp
{
    namespace test
    {
        test_status_t config_t::print_usage(FILE *out, bool detailed)
        {
            ::fputs("USAGE: {utest|ptest|mtest} [args...] [test name...]\n", out);
            if (!detailed)
                return LSP_TEST_FW_INSUFFICIENT;

            char *tempdir = get_default_temporary_path();
            if (!tempdir)
                return LSP_TEST_FW_NO_MEM;

            char *tracepath = get_default_trace_path();
            if (!tempdir)
                return LSP_TEST_FW_NO_MEM;

            char *resdir = get_default_resource_path();
            if (!resdir)
                return LSP_TEST_FW_NO_MEM;

            ::fputs("  First argument:\n", out);
            ::fputs("    utest                 Unit testing subsystem\n", out);
            ::fputs("    ptest                 Performance testing subsystem\n", out);
            ::fputs("    mtest                 Manual testing subsystem\n", out);
            ::fputs("  Additional arguments:\n", out);
            ::fputs("    -a, --args [args...]  Pass arguments to test\n", out);
            ::fputs("    -d, --debug           Disable time restrictions for unit tests\n", out);
            ::fputs("                          for debugging purposes\n", out);
            ::fputs("    -e, --execute         Launch tests specified after this switch\n", out);
            ::fputs("    -f, --fork            Fork child processes (opposite to --nofork)\n", out);
            ::fputs("    -h, --help            Display help\n", out);
            ::fputs("    -i, --ignore          Ignore tests specified after this switch\n", out);
            ::fputs("    -j, --jobs            Set number of job workers for unit tests\n", out);
            ::fputs("    -l, --list            List all available tests\n", out);
        IF_LSP_TEST_FW_PLATFORM_LINUX(
            ::fputs("    -mt, --mtrace         Enable mtrace log\n", out);
        )
            ::fputs("    -nf, --nofork         Do not fork child processes (for better \n", out);
            ::fputs("                          debugging capabilities)\n", out);
        IF_LSP_TEST_FW_PLATFORM_LINUX(
            ::fputs("    -nt, --nomtrace       Disable mtrace log\n", out);
        )
            ::fputs("    -nsi, --nosysinfo     Do not output system information\n", out);
            ::fputs("    -o, --outfile file    Output performance test statistics to specified file\n", out);
            ::fputs("    -r, --resource path   Location of the resource directory used by tests,\n", out);
            ::fprintf(out, "                          default resource path is '%s'\n", resdir);
            ::fputs("    -s, --silent          Do not output additional information from tests\n", out);
            ::fputs("    -ss, --suppress       Suppress extra messages, output only necessary information\n", out);
            ::fputs("    -si, --sysinfo        Output system information\n", out);
            ::fputs("    -t, --tracepath path  Override default trace path with specified value\n", out);
            ::fprintf(out, "                          default trace path is '%s'\n", tracepath);
            ::fputs("    -td, --tempdir path   Override default temporary directory for tests,\n", out);
            ::fprintf(out, "                          default temporary directory is '%s'\n", tempdir);
            ::fputs("    -v, --verbose         Output additional information from tests\n", out);

            ::free(tempdir);
            ::free(tracepath);
            ::free(resdir);

            return LSP_TEST_FW_INSUFFICIENT;
        }

        test_status_t config_t::parse(FILE *out, int argc, const char **argv)
        {
            clear();

    #ifdef LSP_TEST_FW_PLATFORM_WINDOWS
            // Get number of processors for system
            SYSTEM_INFO     os_sysinfo;
            GetSystemInfo(&os_sysinfo);
            threads         = os_sysinfo.dwNumberOfProcessors;

            // Get command line
            LPWSTR cmdline  = GetCommandLineW();
            int nargs = 0;
            LPWSTR *arglist = CommandLineToArgvW(cmdline, &nargs);
            if ((arglist == NULL) || (nargs < 1))
            {
                fprintf(stderr, "Error obtaining command-line arguments\n");
                fflush(stderr);
                return LSP_TEST_FW_UNKNOWN_ERR;
            }

            // Convert UTF-16-encoded command line arguments to UTF-8-encoded
            utf8_argc       = nargs;
            utf8_argv       = reinterpret_cast<char **>(malloc(nargs * sizeof(char *)));
            for (size_t i=0; i<utf8_argc; ++i)
                utf8_argv[i]        = NULL;
            for (size_t i=0; i<utf8_argc; ++i)
            {
                utf8_argv[i]        = utf16_to_utf8(arglist[i]);
                if (utf8_argv[i] == NULL)
                    return LSP_TEST_FW_NO_MEM;
            }

            LocalFree(arglist);

            // Patch arguments
            argc        = utf8_argc;
            argv        = const_cast<const char **>(utf8_argv);
    #else
            threads     = ::sysconf(_SC_NPROCESSORS_ONLN);
    #endif /* LSP_TEST_FW_PLATFORM_WINDOWS */
            if (argc < 2)
                return print_usage(out);

            executable  = argv[0];

            if (!::strcmp(argv[1], "ptest"))
                mode    = PTEST;
            else if (!::strcmp(argv[1], "utest"))
                mode    = UTEST;
            else if (!::strcmp(argv[1], "mtest"))
                mode    = MTEST;
            else if ((!::strcmp(argv[1], "--help")) || ((!::strcmp(argv[1], "-h"))))
                return print_usage(out, true);
            else
                return print_usage(out);

            for (int i=2; i<argc; ++i)
            {
                if ((!::strcmp(argv[i], "--nofork")) || (!::strcmp(argv[i], "-nf")))
                    fork        = false;
                else if ((!::strcmp(argv[i], "--fork")) || (!::strcmp(argv[i], "-f")))
                    fork        = true;
                else if ((!::strcmp(argv[i], "--verbose")) || (!::strcmp(argv[i], "-v")))
                    verbose     = true;
                else if ((!::strcmp(argv[i], "--silent")) || (!::strcmp(argv[i], "-s")))
                    verbose     = false;
                else if ((!::strcmp(argv[i], "--suppress")) || (!::strcmp(argv[i], "-ss")))
                    suppress    = true;
                else if ((!::strcmp(argv[i], "--sysinfo")) || (!::strcmp(argv[i], "-si")))
                    sysinfo     = true;
                else if ((!::strcmp(argv[i], "--nosysinfo")) || (!::strcmp(argv[i], "-nsi")))
                    sysinfo     = false;
                else if ((!::strcmp(argv[i], "--debug")) || (!::strcmp(argv[i], "-d")))
                    debug       = true;
                else if ((!::strcmp(argv[i], "--list")) || (!::strcmp(argv[i], "-l")))
                    list_all    = true;
        #ifdef LSP_TEST_FW_PLATFORM_LINUX
                else if ((!::strcmp(argv[i], "--mtrace")) || (!::strcmp(argv[i], "-mt")))
                    mtrace      = true;
                else if ((!::strcmp(argv[i], "--nomtrace")) || (!::strcmp(argv[i], "-nt")))
                    mtrace      = false;
        #endif /* LSP_TEST_FW_PLATFORM_LINUX */
                else if ((!::strcmp(argv[i], "--tracepath")) || (!::strcmp(argv[i], "-t")))
                {
                    if ((++i) >= argc)
                    {
                        ::fprintf(stderr, "Not specified trace path\n");
                        return LSP_TEST_FW_INVALID_VALUE;
                    }
                    if (tracepath != NULL)
                        ::free(tracepath);
                    if (!(tracepath = ::strdup(argv[i])))
                        return LSP_TEST_FW_NO_MEM;
                }
                else if ((!::strcmp(argv[i], "--tempdir")) || (!::strcmp(argv[i], "-td")))
                {
                    if ((++i) >= argc)
                    {
                        ::fprintf(stderr, "Not specified path to temporary directory\n");
                        return LSP_TEST_FW_INVALID_VALUE;
                    }
                    if (tempdir != NULL)
                        ::free(tempdir);
                    if (!(tempdir = ::strdup(argv[i])))
                        return LSP_TEST_FW_NO_MEM;
                }
                else if ((!::strcmp(argv[i], "--resource")) || (!::strcmp(argv[i], "-r")))
                {
                    if ((++i) >= argc)
                    {
                        ::fprintf(stderr, "Not specified resource path\n");
                        return LSP_TEST_FW_INVALID_VALUE;
                    }
                    if (resource != NULL)
                        ::free(resource);
                    if (!(resource = ::strdup(argv[i])))
                        return LSP_TEST_FW_NO_MEM;
                }
                else if ((!::strcmp(argv[i], "--outfile")) || (!::strcmp(argv[i], "-o")))
                {
                    if ((++i) >= argc)
                    {
                        ::fprintf(stderr, "Not specified name of output file\n");
                        return LSP_TEST_FW_INVALID_VALUE;
                    }
                    outfile     = argv[i];
                }
                else if ((!::strcmp(argv[i], "--args")) || (!::strcmp(argv[i], "-a")))
                {
                    while (++i < argc)
                        args.add(const_cast<char *>(argv[i]));
                }
                else if ((!::strcmp(argv[i], "--jobs")) || (!::strcmp(argv[i], "-j")))
                {
                    if ((++i) >= argc)
                    {
                        fprintf(stderr, "Not specified number of jobs for --jobs parameter\n");
                        return LSP_TEST_FW_INVALID_VALUE;
                    }

                    errno           = 0;
                    char *end       = NULL;
                    long jobs       = ::strtol(argv[i], &end, 10);
                    if ((errno != 0) || ((*end) != '\0') || (jobs <= 0))
                    {
                        ::fprintf(stderr, "Invalid value for --jobs parameter: %s\n", argv[i]);
                        return LSP_TEST_FW_INVALID_VALUE;
                    }
                    threads         = size_t(jobs);
                }
                else if ((!::strcmp(argv[i], "--help")) || ((!::strcmp(argv[i], "-h"))))
                    return print_usage(out, true);
                else if ((!::strcmp(argv[i], "--ignore")) || ((!::strcmp(argv[i], "-i"))))
                    ilist           = true;
                else if ((!::strcmp(argv[i], "--execute")) || ((!::strcmp(argv[i], "-e"))))
                    ilist           = false;
            #ifdef LSP_TEST_FW_PLATFORM_WINDOWS
                else if (!::strcmp(argv[i], "--run-as-nested-process"))
                    is_child        = true;
            #endif /* LSP_TEST_FW_PLATFORM_WINDOWS */
                else
                {
                    if (ilist)
                        ignore.add(const_cast<char *>(argv[i]));
                    else
                        list.add(const_cast<char *>(argv[i]));
                }
            }

            // Initialize default values
            if (!tracepath)
            {
                if (!(tracepath = get_default_trace_path()))
                    return LSP_TEST_FW_NO_MEM;
            }

            if (!tempdir)
            {
                if (!(tempdir = get_default_temporary_path()))
                    return LSP_TEST_FW_NO_MEM;
            }

            if (!resource)
            {
                if (!(resource = get_default_resource_path()))
                    return LSP_TEST_FW_NO_MEM;
            }

            return 0;
        }

        config_t::config_t()
        {
            mode            = UNKNOWN;
            fork            = true;
            verbose         = false;
            debug           = false;
            list_all        = false;
            mtrace          = false;
            ilist           = false;
            sysinfo         = true;
            is_child        = false;
            suppress        = false;
            executable      = NULL;
            tracepath       = NULL;
            resource        = NULL;
            tempdir         = NULL;
            outfile         = NULL;
            threads         = 1;
            this->std_out   = stdout;
            this->std_err   = stderr;

        #ifdef LSP_TEST_FW_PLATFORM_WINDOWS
            utf8_argc       = 0;
            utf8_argv       = NULL;
        #endif /* LSP_TEST_FW_PLATFORM_WINDOWS */
        }

        void config_t::clear()
        {
        #ifdef LSP_TEST_FW_PLATFORM_WINDOWS
            if (utf8_argv != NULL)
            {
                for (size_t i=0; i<utf8_argc; ++i)
                {
                    if (utf8_argv[i] != NULL)
                        ::free(utf8_argv[i]);
                }
                ::free(utf8_argv);

                utf8_argv       = NULL;
                utf8_argc       = 0;
            }
        #endif /* LSP_TEST_FW_PLATFORM_WINDOWS */
            if (tracepath != NULL)
            {
                ::free(tracepath);
                tracepath   = NULL;
            }
            if (resource != NULL)
            {
                ::free(resource);
                resource    = NULL;
            }
            if (tempdir != NULL)
            {
                ::free(tempdir);
                tempdir     = NULL;
            }

            list.clear();
            ignore.clear();
            args.clear();
        }
    }
}


