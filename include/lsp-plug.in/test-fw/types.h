/*
 * types.h
 *
 *  Created on: 21 мар. 2020 г.
 *      Author: sadko
 */

#ifndef LSP_PLUG_IN_TEST_FW_TYPES_H_
#define LSP_PLUG_IN_TEST_FW_TYPES_H_

#include <lsp-plug.in/test-fw/version.h>
#include <lsp-plug.in/common/types.h>
#include <unistd.h>

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
