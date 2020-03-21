/*
 * main.cpp
 *
 *  Created on: 27 июл. 2019 г.
 *      Author: sadko
 */

#include <stdio.h>
#include <string.h>

#include <errno.h>

#include <lsp-plug.in/test-fw/test.h>
#include <lsp-plug.in/test-fw/main/types.h>
#include <lsp-plug.in/test-fw/main/config.h>
#include <lsp-plug.in/test-fw/main/executor.h>

#include <lsp-plug.in/test-fw/ptest.h>
#include <lsp-plug.in/test-fw/mtest.h>
#include <lsp-plug.in/test-fw/utest.h>

//#include <unistd.h>
//#include <sys/time.h>
//#include <stdlib.h>
//#include <signal.h>
//
//#include <test/main/types.h>
//#include <test/main/config.h>
//#include <test/main/executor.h>
//#include <dsp/dsp.h>
//#include <metadata/metadata.h>
//#include <core/stdlib/stdio.h>
//#include <core/init.h>
//#include <sys/stat.h>

namespace lsp
{
    namespace test
    {
        ManualTest *mtest_init()
        {
            return ManualTest::__root;
        }

        UnitTest *utest_init()
        {
            return UnitTest::__root;
        }

        PerformanceTest *ptest_init()
        {
            return PerformanceTest::__root;
        }

        void out_test_header(FILE *out)
        {
            // TODO
        }

        void initialize_global()
        {
        }

        void finalize_global()
        {
        }

        bool match_string(const char *p, const char *m)
        {
            while (p != NULL)
            {
                // Get pattern token
                const char *pnext = ::strchr(p, '.');
                size_t pcount = (pnext == NULL) ? ::strlen(p) : pnext - p;
                if (pnext != NULL)
                    pnext++;

                // Check wildcard
                if ((pcount == 2) && (p[0] == '*') && (p[1] == '*'))
                    return true;

                // Get match token
                if ((m == NULL) || (*m == '\0'))
                    return false;
                const char *mnext = ::strchr(m, '.');
                size_t mcount = (mnext == NULL) ? ::strlen(m) : mnext - m;
                if (mnext != NULL)
                    mnext++;

                // Check wildcard
                if ((pcount != 1) || (p[0] != '*'))
                {
                    // Check that names match
                    if ((pcount != mcount) || (::memcmp(p, m, pcount) != 0))
                        return false;
                }

                // Move pointers
                p = pnext;
                m = mnext;
            }

            return ((m == NULL) || (*m == '\0'));
        }

        bool match_list(dynarray_t &list, test::Test *v, bool match_if_empty)
        {
            // Empty list always matches
            if (list.size() <= 0)
                return match_if_empty;

            const char *full = v->full_name();

            for (size_t i=0, n=list.size(); i<n; ++i)
            {
                if (match_string(list.at<const char>(i), full))
                    return true;
            }

            return false;
        }

        bool check_test_skip(config_t *cfg, stats_t *stats, test::Test *v)
        {
            // Check that test is not ignored
            if (v->ignore())
                return true;

            // Need to check test name and group?
            if (match_list(cfg->ignore, v, false))
            {
                stats->ignored.add(v);
                return true;
            }

            // Is there an explicit list of tests?
            if (!match_list(cfg->list, v, true))
                return true;

            return false;
        }

        static int cmp_test_name(const void *a, const void *b)
        {
            const char *const *_a = reinterpret_cast<const char *const *>(a);
            const char *const *_b = reinterpret_cast<const char *const *>(b);
            return ::strcmp(*_a, *_b);
        }

        status_t list_all(const char *text, test::Test *v)
        {
            dynarray_t names;

            for ( ; v != NULL; v = v->next_test())
            {
                const char *str = v->full_name();
                if (str != NULL)
                    names.add(str);
            }

            size_t n = names.size();
            ::printf("\n%s (total: %d):\n", text, int(n));

            // Sort list of tests and output it to screen
            ::qsort(names.array<void>(), names.size(), sizeof(void *), cmp_test_name);
            for (size_t i=0; i<n; ++i)
                ::printf("  %s\n", names.at<char>(i));
            ::printf("\n");

            names.clear();
            return STATUS_OK;
        }

        status_t output_stats(const config_t *cfg, stats_t *stats)
        {
            const char *tclass =
                    (cfg->mode == UTEST) ? "unit test" :
                    (cfg->mode == PTEST) ? "performance test" :
                    "manual test";

            printf("\n--------------------------------------------------------------------------------\n");
            printf("Overall %s statistics:\n", tclass);
            printf("  execution time [s]:   %.2f\n", stats->overall);
            printf("  launched:             %d\n", int(stats->total));
            printf("  ignored:              %d\n", int(stats->ignored.size()));
            printf("  succeeded:            %d\n", int(stats->success.size()));
            printf("  failed:               %d\n", int(stats->failed.size()));

            if (stats->failed.size() > 0)
            {
                printf("\n--------------------------------------------------------------------------------\n");
                printf("List of failed tests:\n");
                for (size_t i=0, n=stats->failed.size(); i<n; ++i)
                {
                    test::Test *t = stats->failed.at<test::Test>(i);
                    printf("  %s\n", t->full_name());
                }
                printf("\n");
                return STATUS_FAILED;
            }

            if (stats->ignored.size() > 0)
            {
                printf("\n--------------------------------------------------------------------------------\n");
                printf("List of ignored tests:\n");
                for (size_t i=0, n=stats->ignored.size(); i<n; ++i)
                {
                    test::Test *t = stats->ignored.at<test::Test>(i);
                    printf("  %s\n", t->full_name());
                }
            }

            printf("\n");
            return STATUS_OK;
        }

        status_t check_duplicates(const config_t *cfg, test::Test *list)
        {
            const char *tclass =
                    (cfg->mode == UTEST) ? "Unit test" :
                    (cfg->mode == PTEST) ? "Performance test" :
                    "Manual test";

            for (test::Test *first = list; first != NULL; first = first->next_test())
            {
                const char *group = first->group();
                const char *name  = first->name();

                for (test::Test *next = first->next_test(); next != NULL; next = next->next_test())
                {
                    if (strcasecmp(group, next->group()))
                        continue;
                    if (strcasecmp(name, next->name()))
                        continue;

                    fprintf(stderr, "%s '%s.%s' has duplicate instance, can not proceed\n", tclass, group, name);
                    return STATUS_DUPLICATED;
                }
            }

            return STATUS_OK;
        }

        status_t create_outfile(const config_t *cfg)
        {
            if (cfg->outfile == NULL)
                return STATUS_OK;

            FILE *fd = fopen(cfg->outfile, "w");
            if (fd != NULL)
            {
                out_test_header(fd);
                fclose(fd);
            }

            return STATUS_OK;
        }

        int main(int argc, const char **argv)
        {
            config_t cfg;
            status_t res = cfg.parse(stdout, argc, argv);
            if (res != STATUS_OK)
                return res;

            // Output system information
            if (cfg.sysinfo)
                out_test_header(stdout);

            // Obtain the source list of tests for execution
            test::Test *list = NULL;
            switch (cfg.mode)
            {
                case UTEST:
                    list = test::utest_init();
                    if (list == NULL)
                    {
                        fprintf(stderr, "No unit tests available\n");
                        return STATUS_NO_DATA;
                    }
                    else if (cfg.list_all)
                        return list_all("List of available unit tests", list);
                    break;
                case PTEST:
                    list = test::ptest_init();
                    if (list == NULL)
                    {
                        fprintf(stderr, "No performance tests available\n");
                        return STATUS_NO_DATA;
                    }
                    else if (cfg.list_all)
                        return list_all("List of available performance tests", list);
                    break;
                case MTEST:
                    list = test::mtest_init();
                    if (list == NULL)
                    {
                        fprintf(stderr, "No manual tests available\n");
                        return STATUS_NO_DATA;
                    }
                    else if (cfg.list_all)
                        return list_all("List of available manual tests", list);
                    break;
                default:
                    return STATUS_BAD_ARGUMENTS;
            }

            // Perform global initialization
            stats_t stats;
            TestExecutor executor;

            stats.total     = 0;
            stats.overall   = 0.0f;
            res             = executor.init(&cfg, &stats);

            if (res == STATUS_OK)
            {
                // Ensure that there are no duplicates in performance tests
                if (!cfg.is_child)
                {
                    res = check_duplicates(&cfg, list);
                    if (res == STATUS_OK)
                        res     = create_outfile(&cfg);
                }

                // Prepare for test
                if (res == STATUS_OK)
                {
                    test_clock_t start, finish;
                    get_test_time(&start);

                    for (test::Test *v = list; v != NULL; v = v->next_test())
                    {
                        // Check that test is not ignored
                        if (check_test_skip(&cfg, &stats, v))
                            continue;

                        ++stats.total;
                        res = executor.submit(v);
                        if ((res != STATUS_OK) || (cfg.is_child))
                            break;
                    }

                    if (res == STATUS_OK)
                        res = executor.wait();

                    get_test_time(&finish);
                    stats.overall = calc_test_time_difference(&start, &finish);
                }

                // Output statistics
                if (!cfg.is_child)
                    output_stats(&cfg, &stats);
            }

            finalize_global();

            ::fflush(stdout);
            ::fflush(stderr);

            return res;
        }
    }
}
