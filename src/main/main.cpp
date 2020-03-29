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

#include <lsp-plug.in/test-fw/init.h>
#include <lsp-plug.in/test-fw/ptest.h>
#include <lsp-plug.in/test-fw/mtest.h>
#include <lsp-plug.in/test-fw/utest.h>

namespace lsp
{
    namespace test
    {
        status_t check_duplicates(const char *tclass, dynarray_t *list)
        {
            size_t n = list->size();

            for (size_t i=0; i<n; ++i)
            {
                test::Test *a       = list->at<test::Test>(i);
                const char *a_grp   = a->group();
                const char *a_name  = a->name();

                for (size_t j=i+1; j<n; ++j)
                {
                    test::Test *b       = list->at<test::Test>(j);
                    if (::strcasecmp(a_grp, b->group()))
                        continue;
                    if (::strcasecmp(a_name, b->name()))
                        continue;

                    ::fprintf(stderr, "%s '%s.%s' has duplicate instance, can not proceed\n", tclass, a_grp, a_name);
                    return STATUS_DUPLICATED;
                }
            }

            return STATUS_OK;
        }

        status_t mtest_init(dynarray_t *list)
        {
            for (ManualTest *test = ManualTest::__root; test != NULL; test = test->__next)
            {
                if (!list->add(static_cast<Test *>(test)))
                    return STATUS_NO_MEM;
            }

            return check_duplicates("Manual test", list);
        }

        status_t utest_init(dynarray_t *list)
        {
            for (UnitTest *test = UnitTest::__root; test != NULL; test = test->__next)
            {
                if (!list->add(static_cast<Test *>(test)))
                    return STATUS_NO_MEM;
            }

            return check_duplicates("Unit test", list);
        }

        status_t ptest_init(dynarray_t *list)
        {
            for (PerfTest *test = PerfTest::__root; test != NULL; test = test->__next)
            {
                if (!list->add(static_cast<Test *>(test)))
                    return STATUS_NO_MEM;
            }

            return check_duplicates("Performance test", list);
        }

        status_t init_init(dynarray_t *list)
        {
            for (Initializer *init = Initializer::__root; init != NULL; init = init->__next)
            {
                if (!list->add(init))
                    return STATUS_NO_MEM;
            }

            return STATUS_OK;
        }

        void out_system_info(config_t *cfg, dynarray_t *inits)
        {
            for (size_t i=0, n=inits->size(); i<n; ++i)
            {
                test::Initializer *init = inits->at<test::Initializer>(i);
                init->configure(cfg);
                init->info();
            }
        }

        void initialize_global(config_t *cfg, dynarray_t *inits)
        {
            // Initialize in direct order
            for (size_t i=0, n=inits->size(); i<n; ++i)
            {
                test::Initializer *init = inits->at<test::Initializer>(i);
                init->configure(cfg);
                init->initialize();
            }
        }

        void finalize_global(dynarray_t *inits)
        {
            // Finalize in reverse order
            for (ssize_t i=inits->size()-1; i>=0; --i)
            {
                test::Initializer *init = inits->at<test::Initializer>(i);
                init->finalize();
            }
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

        status_t list_all(const char *text, dynarray_t *list)
        {
            dynarray_t names;

            for (size_t i=0, n=list->size() ; i < n; ++i)
            {
                test::Test *test = list->at<test::Test>(i);
                const char *str = test->full_name();
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

        status_t create_outfile(config_t *cfg, dynarray_t *inits)
        {
            if (cfg->outfile == NULL)
                return STATUS_OK;

            FILE *fd = fopen(cfg->outfile, "w");
            if (fd != NULL)
            {
                // Update configuration
                FILE *std_out   = cfg->std_out;
                bool verbose    = cfg->verbose;
                cfg->std_out    = fd;
                cfg->verbose    = true;

                // Output information
                out_system_info(cfg, inits);

                // Restore configuration
                cfg->std_out    = std_out;
                cfg->verbose    = verbose;

                fclose(fd);
            }

            return STATUS_OK;
        }

        LSP_SYMBOL_EXPORT int main(int argc, const char **argv)
        {
            // Parse configuration
            config_t cfg;
            status_t res = cfg.parse(stdout, argc, argv);
            if (res != STATUS_OK)
                return res;

            // Initialize list of test initializers
            dynarray_t inits;
            res = init_init(&inits);
            if (res != STATUS_OK)
            {
                ::fprintf(stderr, "Failed to initialize initializer list\n");
                return res;
            }

            // Perform global initialization
            initialize_global(&cfg, &inits);
            if (cfg.sysinfo)
                out_system_info(&cfg, &inits);

            // Obtain the source list of tests for execution
            dynarray_t list;    // List of tests
            switch (cfg.mode)
            {
                case UTEST:
                    res = test::utest_init(&list);
                    if (res != STATUS_OK)
                    {
                        ::fprintf(stderr, "Error initializing unit test subsystem\n");
                        return res;
                    }
                    else if (list.is_empty())
                    {
                        ::fprintf(stderr, "No unit tests available\n");
                        return STATUS_NO_DATA;
                    }
                    else if (cfg.list_all)
                        return list_all("List of available unit tests", &list);
                    break;
                case PTEST:
                    res = test::ptest_init(&list);
                    if (res != STATUS_OK)
                    {
                        ::fprintf(stderr, "Error initializing performance test subsystem\n");
                        return res;
                    }
                    else if (list.is_empty())
                    {
                        fprintf(stderr, "No performance tests available\n");
                        return STATUS_NO_DATA;
                    }
                    else if (cfg.list_all)
                        return list_all("List of available performance tests", &list);
                    break;
                case MTEST:
                    res = test::mtest_init(&list);
                    if (res != STATUS_OK)
                    {
                        ::fprintf(stderr, "Error initializing unit test subsystem\n");
                        return res;
                    }
                    else if (list.is_empty())
                    {
                        fprintf(stderr, "No manual tests available\n");
                        return STATUS_NO_DATA;
                    }
                    else if (cfg.list_all)
                        return list_all("List of available manual tests", &list);
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
                    res     = create_outfile(&cfg, &inits);

                // Prepare for test
                if (res == STATUS_OK)
                {
                    test_clock_t start, finish;
                    get_test_time(&start);

                    for (size_t i=0, n=list.size(); i<n; ++i)
                    {
                        test::Test *test = list.at<test::Test>(i);

                        // Check that test is not ignored
                        if (check_test_skip(&cfg, &stats, test))
                            continue;

                        ++stats.total;
                        res = executor.submit(test);
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

            finalize_global(&inits);

            ::fflush(stdout);
            ::fflush(stderr);

            return res;
        }
    }
}
