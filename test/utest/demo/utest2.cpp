/*
 * utest2.cpp
 *
 *  Created on: 22 мар. 2020 г.
 *      Author: sadko
 */

#include <lsp-plug.in/test-fw/utest.h>
#include <lsp-plug.in/test-fw/FloatBuffer.h>

UTEST_BEGIN("demo", utest2)

    UTEST_MAIN
    {
        FloatBuffer buf1(0x20);
        FloatBuffer buf2(buf1);

        UTEST_ASSERT(buf1.equals_relative(buf2));
        UTEST_ASSERT(buf1.valid());

        buf1.dump("buf1");
        buf1.dump("buf2");
    }

UTEST_END



