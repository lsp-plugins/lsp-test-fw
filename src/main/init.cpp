/*
 * init.cpp
 *
 *  Created on: 22 мар. 2020 г.
 *      Author: sadko
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


