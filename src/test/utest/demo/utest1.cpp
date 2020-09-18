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


