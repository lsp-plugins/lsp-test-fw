/*
 * test.h
 *
 *  Created on: 22 авг. 2018 г.
 *      Author: vsadovnikov
 */

#ifndef TEST_TEST_H_
#define TEST_TEST_H_

/**
 * This macro allows to export function/pointer as supported
 * for further testing
 */
#define TEST_EXPORT(ptr)            ::test::Test::mark_supported(ptr)

/**
 * This macro allows to check function/pointer as supported
 * for further testing
 */
#define TEST_SUPPORTED(ptr)         ::test::Test::check_supported(ptr)

/**
 * Do an assertion
 */
#define TEST_ASSERT(code) \
        if (!(code)) { \
            fprintf(stderr, "Test assertion has failed at file %s, line %d:\n  %s\n", \
                    __FILE__, __LINE__, # code); \
            exit(2); \
        }

#include <lsp-plug.in/test-fw/main/dynarray.h>

namespace lsp
{
    namespace test
    {
        class Test
        {
            private:
                static dynarray_t   support;
                static void         __mark_supported(const void *x);
                static bool         __check_supported(const void *ptr);

            protected:
                const char         *__test_group;
                const char         *__test_name;
                mutable char       *__full_name;
                bool                __verbose;
                const char         *__executable;

            public:
                int                 printf(const char *fmt, ...);
                int                 eprintf(const char *fmt, ...);

            public:
                inline const char  *name() const            { return __test_name; }
                inline const char  *group() const           { return __test_group; }
                inline const char  *executable() const      { return __executable; }
                const char         *full_name() const;

            public:
                explicit Test(const char *group, const char *name);
                virtual ~Test();

            public:
                inline void         set_verbose(bool verbose)       { __verbose = verbose; }
                inline void         set_executable(const char *name){ __executable = name; }

                virtual void        execute(int argc, const char **argv) = 0;

                virtual void        init();

                virtual bool        ignore() const;

                virtual void        destroy();

                virtual Test       *next_test() const = 0;

            public:
                template <typename T>
                    static inline void     mark_supported(T x)
                    {
                        __mark_supported(reinterpret_cast<const void *>(x));
                    }

                template <typename T>
                    static bool     check_supported(T x)
                    {
                        return __check_supported(reinterpret_cast<const void *>(x));
                    }

        };
    }
}

#endif /* TEST_TEST_H_ */
