/*
 * Copyright (C) 2020 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2020 Vladimir Sadovnikov <sadko4u@gmail.com>
 *
 * This file is part of lsp-test-fw
 * Created on: 16 авг. 2018 г.
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

#ifndef TEST_PTEST_H_
#define TEST_PTEST_H_

#include <lsp-plug.in/test-fw/version.h>
#include <lsp-plug.in/test-fw/test.h>

#include <stdio.h>
#include <time.h>

#define PTEST_BEGIN(group, name, time, iterations) \
        namespace lsp { \
        namespace ptest { \
        namespace { \
            \
            using namespace ::lsp::test; \
            \
            class ptest_ ## name: public PerfTest { \
                private: \
                    ptest_ ## name & operator = (const ptest_ ## name &); \
                \
                public: \
                    typedef ptest_ ## name test_type_t; \
                \
                public: \
                    explicit ptest_ ## name() : PerfTest(group, #name, time, iterations) {} \
                    \
                    virtual ~ptest_ ## name() {}

#define PTEST_IGNORE \
        virtual bool ignore() const { return true; }

#define PTEST_MAIN \
        virtual void execute(int argc, const char **argv)

#define PTEST_SUPPORTED(ptr)        TEST_SUPPORTED(ptr)

#define PTEST_LOOP(__key, ...) { \
        double __start = clock(); \
        double __time = 0.0f; \
        wsize_t __iterations = 0; \
        \
        do { \
            for (size_t __i=0; __i<__test_iterations; ++__i) { \
                __VA_ARGS__; \
            } \
            /* Calculate statistics */ \
            __iterations   += __test_iterations; \
            __time          = (clock() - __start) / CLOCKS_PER_SEC; \
        } while (__time < __test_time); \
        \
        gather_stats(__key, __time, __iterations); \
        if (__verbose) { \
            printf("  time [s]:                 %.2f/%.2f\n", __time, __test_time); \
            printf("  iterations:               %ld/%ld\n", long(__iterations), long((__iterations * __test_time) / __time)); \
            printf("  performance [i/s]:        %.2f\n", __iterations / __time); \
            printf("  iteration time [us/i]:    %.4f\n\n", (1000000.0 * __time) / __iterations); \
        } \
    }

#define PTEST_KLOOP(__key, __mul, ...) { \
        double __start = clock(); \
        double __time = 0.0f; \
        wsize_t __iterations = 0; \
        wsize_t __k_iterations = __test_iterations; \
        \
        do { \
            for (size_t __i=0; __i<__k_iterations; ++__i) { \
                __VA_ARGS__; \
            } \
            /* Calculate statistics */ \
            __iterations   += __k_iterations; \
            __time          = (clock() - __start) / CLOCKS_PER_SEC; \
        } while (__time < __test_time); \
        \
        gather_stats(__key, __time, __iterations); \
        if (__verbose) { \
            printf("  time [s]:                 %.2f/%.2f\n", __time, __test_time); \
            printf("  iterations:               %ld/%ld\n", long(__iterations), long((__iterations * __test_time) / __time)); \
            printf("  performance [i/s]:        %.2f\n", __iterations / __time); \
            printf("  iteration time [us/i]:    %.4f\n\n", (1000000.0 * __time) / __iterations); \
        } \
    }

#define PTEST_SEPARATOR \
        gather_stats(NULL, 0.0f, 0); \
        printf("\n");

#define PTEST_SEPARATOR2 \
        gather_stats(NULL, 0.0f, 1); \
        printf("\n");

#define PTEST_FAIL_MSG(message, ...) do {  \
            fprintf(stderr, "Performance test '%s.%s' has failed at file %s, line %d with message:\n  " message  "\n", \
                    __test_group, __test_name, __FILE__, __LINE__, ## __VA_ARGS__); \
            exit(1); \
        } while (false)

#define PTEST_FAIL() do {\
            fprintf(stderr, "Performance test '%s.%s' has failed at file %s, line %d\n", \
                    __test_group, __test_name, __FILE__, __LINE__); \
            exit(1); \
        } while (false)

#define PTEST_END \
        } performance_test;  /* ptest class */ \
        } /* anonymous namespace */ \
        } /* namespace ptest */ \
        } /* namespace lsp */

namespace lsp
{
    namespace test
    {
        class LSP_TEST_FW_PUBLIC PerfTest: public Test
        {
            private:
                PerfTest & operator = (const PerfTest &);

            private:
                friend test_status_t    ptest_init(dynarray_t *list);

            private:
                static PerfTest    *__root;
                PerfTest           *__next;

            protected:
                typedef struct stats_t
                {
                    char       *key;            /* The loop indicator */
                    char       *time;           /* Actual time [seconds] */
                    char       *n_time;         /* Normalized time [seconds] */
                    char       *iterations;     /* Number of iterations */
                    char       *n_iterations;   /* Normalized number of iterations */
                    char       *performance;    /* The performance of test [iterations per second] */
                    char       *time_cost;      /* The amount of time spent per iteration [milliseconds per iteration] */
                    char       *rel;            /* The relative speed */
                    double      cost;           /* The overall cost */
                } stats_t;

            protected:
                size_t                              __test_iterations;
                double                              __test_time;
                mutable dynarray_t                  __test_stats;

            protected:
                void                gather_stats(const char *key, double time, wsize_t iterations);

            private:
                static void         destroy_stats(stats_t *stats);
                static stats_t     *alloc_stats();
                static void         estimate(size_t *len, const char *text);
                static void         out_text(FILE *out, size_t length, const char *text, int align, const char *padding, const char *tail);

            public:
                int                 printf(const char *fmt, ...);

            public:
                explicit PerfTest(const char *group, const char *name, float time, size_t iterations);
                virtual ~PerfTest();

            public:
                void                dump_stats(FILE *out) const;
                void                free_stats();
        };
    } /* namespace test */
} /* namespace lsp */

#endif /* TEST_PTEST_H_ */
