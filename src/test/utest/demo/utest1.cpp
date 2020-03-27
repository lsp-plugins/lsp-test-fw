/*
 * utest1.cpp
 *
 *  Created on: 22 мар. 2020 г.
 *      Author: sadko
 */


#include <lsp-plug.in/test-fw/utest.h>

UTEST_BEGIN("demo", utest1)

    UTEST_MAIN
    {
        int i = 5;
        printf("  i = %d\n", i); // Will be printed in verbose mode
        int j = 6;
        printf("  j = %d\n", j); // Will be printed in verbose mode

        UTEST_ASSERT(i == 5);
        UTEST_ASSERT(j == 6);

        i += j;

        UTEST_ASSERT(i == 11);
        UTEST_ASSERT(j == 6);
    }

UTEST_END


