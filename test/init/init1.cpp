/*
 * init1.cpp
 *
 *  Created on: 22 мар. 2020 г.
 *      Author: sadko
 */

#include <lsp-plug.in/test-fw/init.h>

INIT_BEGIN(demo_initializer1)

    INIT_FUNC
    {
        printf("Running test initializer 1");
    }

    INFO_FUNC
    {
        printf("Some additional information from initializer 1");
    }

    FINI_FUNC
    {
        printf("Running test finalizer 1");
    }

INIT_END


