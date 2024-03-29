/*
 * Copyright (C) 2020 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2020 Vladimir Sadovnikov <sadko4u@gmail.com>
 *
 * This file is part of lsp-test-fw
 * Created on: 24 авг. 2018 г.
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

#ifndef TEST_MTEST_H_
#define TEST_MTEST_H_

#include <lsp-plug.in/test-fw/version.h>
#include <lsp-plug.in/test-fw/test.h>
#include <lsp-plug.in/test-fw/main/dynarray.h>

#define MTEST_BEGIN(group, name) \
        namespace lsp { \
        namespace mtest { \
        namespace { \
            \
            using namespace ::lsp::test; \
            \
            class mtest_ ## name: public ManualTest { \
                private: \
                    mtest_ ## name & operator = (const mtest_ ## name &); \
                \
                public: \
                    typedef mtest_ ## name test_type_t; \
                \
                public: \
                    explicit mtest_ ## name() : ManualTest(group, #name) {} \
                    \
                    virtual ~mtest_ ## name() {}

#define MTEST_MAIN \
        virtual void execute(int argc, const char **argv)

#define MTEST_SUPPORTED(ptr)        TEST_SUPPORTED(ptr)

#define MTEST_FAIL_MSG(message, ...) do {  \
            fprintf(stderr, "Manual test '%s.%s' has failed at file %s, line %d with message: \n  " message  "\n", \
                    __test_group, __test_name, __FILE__, __LINE__, ## __VA_ARGS__); \
            exit(1); \
        } while (false)

#define MTEST_FAIL(...) do {\
            fprintf(stderr, "Manual test '%s.%s' has failed at file %s, line %d\n", \
                    __test_group, __test_name, __FILE__, __LINE__); \
            __VA_ARGS__; \
            exit(1); \
        } while (false)

#define MTEST_FAIL_SILENT()     exit(5);

#define MTEST_ASSERT(code, ...) do { \
            if (!(code)) { \
                fprintf(stderr, "Manual test '%s.%s' assertion has failed at file %s, line %d:\n  %s\n", \
                        __test_group, __test_name, __FILE__, __LINE__, # code); \
                __VA_ARGS__; \
                exit(2); \
            } \
        } while (false)

#define MTEST_ASSERT_MSG(code, message, ...) do { \
            if (!(code)) { \
                fprintf(stderr, "Manual test '%s.%s' assertion has failed at file %s, line %d:\n  %s\n  " message "\n", \
                        __test_group, __test_name, __FILE__, __LINE__, # code, ## __VA_ARGS__); \
                exit(2); \
            } \
        } while (false)

#define MTEST_END \
        } manual_test;  /* mtest class */ \
        } /* anonymous namespace */ \
        } /* namespace mtest */ \
        } /* namespace lsp */

namespace lsp
{
    namespace test
    {
        class LSP_TEST_FW_PUBLIC ManualTest: public Test
        {
            private:
                ManualTest & operator = (const ManualTest &);

            private:
                friend test_status_t    mtest_init(dynarray_t *list);

            private:
                static ManualTest      *__root;
                ManualTest             *__next;

            public:
                explicit ManualTest(const char *group, const char *name);
                virtual ~ManualTest();
        };
    } /* namespace test */
} /* namespace lsp */

#endif /* TEST_MTEST_H_ */
