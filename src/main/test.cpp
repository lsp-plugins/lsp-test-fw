/*
 * Copyright (C) 2020 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2020 Vladimir Sadovnikov <sadko4u@gmail.com>
 *
 * This file is part of lsp-test-fw
 * Created on: 22 авг. 2018 г.
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

#include <stdio.h>

#include <lsp-plug.in/test-fw/test.h>
#include <lsp-plug.in/test-fw/main/tools.h>

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
                    int n = asprintf(&__full_name, "%s.%s", __test_group, __test_name);
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



