/*
 * main.h
 *
 *  Created on: 27 июл. 2019 г.
 *      Author: sadko
 */

#ifndef TEST_MAIN_H_
#define TEST_MAIN_H_

#include <lsp-plug.in/test-fw/version.h>
#include <lsp-plug.in/common/types.h>

namespace lsp
{
    namespace test
    {
        /**
         * The main function that executes the test framework
         * @param argc number of arguments
         * @param argv list of arguments
         * @return execution code
         */
        LSP_SYMBOL_EXPORT int main(int argc, const char **argv);
    }
}


#endif /* TEST_MAIN_H_ */
