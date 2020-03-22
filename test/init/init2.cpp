/*
 * init2.cpp
 *
 *  Created on: 22 мар. 2020 г.
 *      Author: sadko
 */

#include <lsp-plug.in/test-fw/init.h>

INIT_BEGIN(demo_initializer2)

    INIT_FUNC
    {
        printf("Running test initializer 2");
    }

    INFO_FUNC
    {
        printf("Some additional information from initializer 2");
    }

    FINI_FUNC
    {
        printf("Running test finalizer 2");
    }

INIT_END


