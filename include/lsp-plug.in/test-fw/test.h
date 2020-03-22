/*
 * test.h
 *
 *  Created on: 22 авг. 2018 г.
 *      Author: vsadovnikov
 */

#ifndef TEST_TEST_H_
#define TEST_TEST_H_


#include <lsp-plug.in/test-fw/env.h>
#include <lsp-plug.in/test-fw/main/dynarray.h>

/**
 * This macro allows to export function/pointer as supported
 * for further testing
 */
#define TEST_EXPORT(ptr)            ::lsp::test::Test::mark_supported(ptr)

/**
 * This macro allows to check function/pointer as supported
 * for further testing
 */
#define TEST_SUPPORTED(ptr)         ::lsp::test::Test::check_supported(ptr)

/**
 * Do an assertion
 */
#define TEST_ASSERT(code) \
        if (!(code)) { \
            fprintf(stderr, "Test assertion has failed at file %s, line %d:\n  %s\n", \
                    __FILE__, __LINE__, # code); \
            exit(2); \
        }

namespace lsp
{
    namespace test
    {
        class Test: public Environment
        {
            private:
                static dynarray_t   support;
                static void         __mark_supported(const void *x);
                static bool         __check_supported(const void *ptr);

            protected:
                const char         *__test_group;
                const char         *__test_name;
                mutable char       *__full_name;

            public:
                inline const char  *name() const            { return __test_name; }
                inline const char  *group() const           { return __test_group; }
                const char         *full_name() const;

            public:
                explicit Test(const char *group, const char *name);
                virtual ~Test();

            public:
                virtual void        execute(int argc, const char **argv) = 0;

                virtual void        init();

                virtual bool        ignore() const;

                virtual void        destroy();

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
