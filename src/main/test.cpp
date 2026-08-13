/*
 * Copyright (C) 2026 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2026 Vladimir Sadovnikov <sadko4u@gmail.com>
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
        Test::support_list_t    Test::support   = { NULL, 0, 0 };

        Test::support_list_t::~support_list_t()
        {
            if (list != NULL)
                free(list);
            list                = NULL;
            count               = 0;
            capacity            = 0;
        }

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
            if (support.count >= support.capacity)
            {
                uint32_t new_cap    = support.capacity + (support.capacity >> 1);
                if (new_cap < 0x20)
                    new_cap             = 0x20;
                const void **data       = static_cast<const void **>(::realloc(support.list, sizeof(const void *) * new_cap));
                if (data == NULL)
                    return;

                support.list        = data;
                support.capacity    = new_cap;
            }

            support.list[support.count++]  = const_cast<void *>(ptr);
        }

        bool Test::__check_supported(const void *ptr)
        {
            for (uint32_t i=0; i<support.count; ++i)
                if (support.list[i] == ptr)
                    return true;
            return false;
        }
    } /* namespace test */
} /* namespace lsp */



