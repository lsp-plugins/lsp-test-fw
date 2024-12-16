/*
 * Copyright (C) 2020 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2020 Vladimir Sadovnikov <sadko4u@gmail.com>
 *
 * This file is part of lsp-test-fw
 * Created on: 22 мар. 2020 г.
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

#include <math.h>

#include <lsp-plug.in/test-fw/ptest.h>
#include <lsp-plug.in/test-fw/helpers.h>

namespace
{
    void bulk_sin(float *dst, const float *src, size_t n)
    {
        for (size_t i=0; i<n; ++i)
            dst[i] = sinf(src[i]);
    }

    void bulk_cos(float *dst, const float *src, size_t n)
    {
        for (size_t i=0; i<n; ++i)
            dst[i] = cosf(src[i]);
    }

    void bulk_tan(float *dst, const float *src, size_t n)
    {
        for (size_t i=0; i<n; ++i)
            dst[i] = tanf(src[i]);
    }
}

typedef void (* test_func_t)(float *dst, const float *src, size_t n);

PTEST_BEGIN("demo", trig, 2.0, 10000)

    void call(const char *label, float *dst, const float *src, size_t count, test_func_t func)
    {
        // Some functions may be not supported due some reasons, do not launch test
        // for non-supported functions
        //
        // For example, there can be an AVX implemenation of some stuff
        // but we execute the test on machine without AVX
        // If we detect the AVX support somewhere where we define the function,
        // we mark it as exported for tests by calling TEST_EXPORT(function).
        // Otherwise PTEST_SUPPORTED(function) will return false.
        if (!PTEST_SUPPORTED(func))
            return;

        char buf[80];
        snprintf(buf, sizeof(buf), "%s x %d", label, int(count));
        printf("Testing %s numbers...\n", buf);

        PTEST_LOOP(buf,
            func(dst, src, count);
        );
    }

    PTEST_MAIN
    {
        float in_buf[0x1000];
        float out_buf[0x1000];

        // We call TEST_EXPORT() to make each function visible for PTEST_SUPPORTED() call
        TEST_EXPORT(bulk_sin);
        TEST_EXPORT(bulk_cos);
        TEST_EXPORT(bulk_tan);

        randomize(in_buf, 0x1000, -1.0f, 1.0f);

        for (size_t buf_size = 0x100; buf_size <= 0x1000; buf_size <<= 2)
        {
            call("bulk_sin", out_buf, in_buf, buf_size, bulk_sin);
            call("bulk_cos", out_buf, in_buf, buf_size, bulk_cos);
            call("bulk_tan", out_buf, in_buf, buf_size, bulk_tan);

            PTEST_SEPARATOR;
        }
    }

PTEST_END


