/*
 * Copyright (C) 2020 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2020 Vladimir Sadovnikov <sadko4u@gmail.com>
 *
 * This file is part of lsp-test-fw
 * Created on: 19 авг. 2018 г.
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

#ifndef TEST_UTEST_H_
#define TEST_UTEST_H_

#include <lsp-plug.in/test-fw/version.h>
#include <lsp-plug.in/test-fw/test.h>
#include <lsp-plug.in/test-fw/main/dynarray.h>

#define UTEST_BEGIN(group, name) \
        namespace lsp { \
        namespace utest { \
        namespace { \
            \
            using namespace ::lsp::test; \
            \
            class utest_ ## name: public UnitTest { \
                private: \
                    utest_ ## name & operator = (const utest_ ## name &); \
                \
                public: \
                    typedef utest_ ## name test_type_t;\
                \
                public: \
                    \
                    _Pragma("GCC diagnostic push") \
                    _Pragma("GCC diagnostic ignored \"-Wuninitialized\"") \
                    explicit utest_ ## name() : UnitTest(group, #name) {} \
                    _Pragma("GCC diagnostic pop") \
                    \
                    virtual ~utest_ ## name() {}

#define UTEST_TIMELIMIT(seconds) \
        virtual double time_limit() const { return double(seconds); }

#define UTEST_IGNORE \
        virtual bool ignore() const { return true; }

#define UTEST_MAIN \
        virtual void execute(int argc, const char **argv)

#define UTEST_INIT \
        virtual void init()

#define UTEST_DESTROY \
        virtual void destroy()

#define UTEST_SUPPORTED(ptr)        TEST_SUPPORTED(ptr)

#define UTEST_FAIL_MSG(message, ...) {  \
            ::fprintf(stderr, "Unit test '%s.%s' has failed at file %s, line %d with message: \n  " message  "\n", \
                    __test_group, __test_name, __FILE__, __LINE__, ## __VA_ARGS__); \
            ::exit(1); \
        }

#define UTEST_FAIL(...) {\
            ::fprintf(stderr, "Unit test '%s.%s' has failed at file %s, line %d\n", \
                    __test_group, __test_name, __FILE__, __LINE__); \
            __VA_ARGS__; \
            ::exit(1); \
        }

#define UTEST_ASSERT(code, ...) \
        if (!(code)) { \
            ::fprintf(stderr, "Unit test '%s.%s' assertion has failed at file %s, line %d:\n  %s\n", \
                    __test_group, __test_name, __FILE__, __LINE__, # code); \
            __VA_ARGS__; \
            ::exit(2); \
        }

#define UTEST_ASSERT_MSG(code, message, ...) \
        if (!(code)) { \
            ::fprintf(stderr, "Unit test '%s.%s' assertion has failed at file %s, line %d:\n  %s\n  " message "\n", \
                    __test_group, __test_name, __FILE__, __LINE__, # code, ## __VA_ARGS__); \
            ::exit(2); \
        }

#define UTEST_FOREACH(var, ...)    \
        const size_t ___sizes[] = { __VA_ARGS__, 0 }; \
        for (size_t ___i=0, var=___sizes[0]; ___i<(sizeof(___sizes)/sizeof(size_t) - 1); var=___sizes[++___i])

#define UTEST_END \
        } unit_test;  /* utest class */ \
        } /* anonymous namespace */ \
        } /* namespace utest */ \
        } /* namespace lsp */

namespace lsp
{
    namespace test
    {
        class UnitTest: public Test
        {
            private:
                UnitTest & operator = (const UnitTest &);

            private:
                friend test_status_t    utest_init(dynarray_t *list);

            private:
                static UnitTest        *__root;
                UnitTest               *__next;

            public:
                explicit UnitTest(const char *group, const char *name);
                virtual ~UnitTest();

            public:
                virtual double time_limit() const;
        };
    }
}

#endif /* TEST_UTEST_H_ */
