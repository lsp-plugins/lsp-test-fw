/*
 * utest.cpp
 *
 *  Created on: 19 авг. 2018 г.
 *      Author: sadko
 */

#include <lsp-plug.in/test-fw/utest.h>

namespace lsp
{
    namespace test
    {
        UnitTest   *UnitTest::__root = NULL;

        UnitTest::UnitTest(const char *group, const char *name): Test(group, name)
        {
            // Self-register
            __next              = __root;
            __root              = this;
        }

        UnitTest::~UnitTest()
        {
        }

        double UnitTest::time_limit() const
        {
            return 30.0;
        }

    }
}
