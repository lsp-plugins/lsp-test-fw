/*
 * mtest1.cpp
 *
 *  Created on: 30 мар. 2020 г.
 *      Author: sadko
 */


#include <lsp-plug.in/test-fw/mtest.h>

#include <stdio.h>
#include <string.h>

MTEST_BEGIN("demo", mtest1)

    MTEST_MAIN
    {
        char buffer[0x100];
        // Check number of arguments
        MTEST_ASSERT_MSG(argc > 0, "Insufficient arguments");

        ::printf("Enter the string that matches first argument:\n");
        ::scanf("%s", buffer);

        // Validate
        MTEST_ASSERT_MSG(::strcmp(buffer, argv[0]) == 0, "Invalid value, you should enter: %s", argv[0]);
    }

MTEST_END
