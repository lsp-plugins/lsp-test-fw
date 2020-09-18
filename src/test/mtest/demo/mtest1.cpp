/*
 * Copyright (C) 2020 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2020 Vladimir Sadovnikov <sadko4u@gmail.com>
 *
 * This file is part of lsp-test-fw
 * Created on: 30 мар. 2020 г.
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
