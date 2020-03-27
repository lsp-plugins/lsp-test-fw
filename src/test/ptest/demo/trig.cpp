/*
 * trig.cpp
 *
 *  Created on: 22 мар. 2020 г.
 *      Author: sadko
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
        if (!PTEST_SUPPORTED(func))
            return;

        char buf[80];
        sprintf(buf, "%s x %d", label, int(count));
        printf("Testing %s numbers...\n", buf);

        PTEST_LOOP(buf,
            func(dst, src, count);
        );
    }

    PTEST_MAIN
    {
        float in_buf[0x1000];
        float out_buf[0x1000];

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


