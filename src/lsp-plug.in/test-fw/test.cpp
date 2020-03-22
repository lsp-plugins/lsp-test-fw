/*
 * test.cpp
 *
 *  Created on: 22 авг. 2018 г.
 *      Author: vsadovnikov
 */

#include <stdio.h>

#include <lsp-plug.in/test-fw/test.h>

namespace lsp
{
    namespace test
    {
        dynarray_t      Test::support;

        Test::Test(const char *group, const char *name)
        {
            __test_group        = group;
            __test_name         = name;
            __full_name         = NULL;
        }

        Test::~Test()
        {
            if ((__full_name != NULL) && (__full_name != __test_name))
            {
                free(__full_name);
                __full_name = NULL;
            }
        }

        const char *Test::full_name() const
        {
            if (__full_name == NULL)
            {
                if ((__test_group != NULL) && (::strlen(__test_group) > 0))
                {
                    int n = ::asprintf(&__full_name, "%s.%s", __test_group, __test_name);
                    if (n < 0)
                        return NULL;
                }
                if (__full_name == NULL)
                    __full_name         = const_cast<char *>(__test_name);
            }
            return __full_name;
        }

        bool Test::ignore() const
        {
            return false;
        }

        void Test::init()
        {
        }

        void Test::destroy()
        {
        }

        void Test::__mark_supported(const void *ptr)
        {
            support.add(const_cast<void *>(ptr));
        }

        bool Test::__check_supported(const void *ptr)
        {
            return support.index_of(ptr) >= 0;
        }
    }
}



