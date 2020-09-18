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

#include <lsp-plug.in/test-fw/init.h>

namespace lsp
{
    namespace test
    {
        Initializer *Initializer::__root = NULL;

        Initializer::Initializer(const char *name)
        {
            // Self-register
            __next              = __root;
            __root              = this;
            __name              = name;
        }

        Initializer::~Initializer()
        {
        }

        void Initializer::initialize()
        {
        }

        void Initializer::info()
        {
        }

        void Initializer::finalize()
        {
        }

        void Initializer::before(const char *test, test_mode_t type)
        {
        }

        void Initializer::after(const char *test, test_mode_t type)
        {
        }
    }
}


