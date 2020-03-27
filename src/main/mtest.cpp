/*
 * mtest.cpp
 *
 *  Created on: 24 авг. 2018 г.
 *      Author: sadko
 */

#include <lsp-plug.in/test-fw/mtest.h>

namespace lsp
{
    namespace test
    {
        ManualTest *ManualTest::__root = NULL;

        ManualTest::ManualTest(const char *group, const char *name):
            Test(group, name)
        {
            // Self-register
            __next              = __root;
            __root              = this;
        }

        ManualTest::~ManualTest()
        {
        }
    }
}
