/*
 * types.h
 *
 *  Created on: 21 мар. 2020 г.
 *      Author: sadko
 */

#ifndef LSP_PLUG_IN_TEST_FW_TYPES_H_
#define LSP_PLUG_IN_TEST_FW_TYPES_H_

// Define header version
#define LSP_TEST_FW_MAJOR           1
#define LSP_TEST_FW_MINOR           0
#define LSP_TEST_FW_MICRO           0

#include <unistd.h>

#include <lsp-plug.in/common/types.h>

namespace lsp
{
    namespace test
    {
        template <class T>
            inline T *align_pointer(void *src, size_t align)
            {
                ptrdiff_t x     = ptrdiff_t(src);
                ptrdiff_t off   = (x % align);

                return (off == 0) ?
                        reinterpret_cast<T *>(src) :
                        reinterpret_cast<T *>(x + align - off);
            }

        template <class T>
            inline bool check_alignment(const T *src, size_t align)
            {
                ptrdiff_t x     = ptrdiff_t(src);
                return !(x % align);
            }
    }
}

#endif /* LSP_PLUG_IN_TEST_FW_TYPES_H_ */
