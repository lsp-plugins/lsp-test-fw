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

#ifndef LSP_PLUG_IN_TEST_FW_DYNARRAY_H_
#define LSP_PLUG_IN_TEST_FW_DYNARRAY_H_

#include <lsp-plug.in/test-fw/version.h>
#include <lsp-plug.in/test-fw/types.h>

#include <stdlib.h>
#include <string.h>

namespace lsp
{
    namespace test
    {
        /**
         * Implementation of simple dynamic array of pointers,
         * to avoid STL and std::vector stuff
         */
        typedef class dynarray_t
        {
            private:
                dynarray_t & operator = (const dynarray_t &);

            private:
                void      **pData;
                size_t      nSize;
                size_t      nCapacity;

            public:
                inline bool add(const void *ptr)
                {
                    if (nSize >= nCapacity)
                    {
                        void *data      = ::realloc(pData, sizeof(void *) * (nCapacity + 0x20));
                        if (data == NULL)
                            return false;

                        pData           = static_cast<void **>(data);
                        nCapacity      += 0x20;
                    }

                    pData[nSize++]  = const_cast<void *>(ptr);
                    return true;
                }

                inline bool remove(size_t idx)
                {
                    if (idx >= nSize)
                        return false;

                    if (idx < (--nSize))
                        ::memmove(&pData[idx], &pData[idx+1], (nSize - idx) * sizeof(void *));

                    pData[nSize] = NULL;
                    return true;
                }

                inline bool remove(const void *ptr)
                {
                    for (size_t i=0; i<nSize; ++i)
                    {
                        if (pData[i] == ptr)
                            return remove(i);
                    }
                    return false;
                }

                inline ssize_t index_of(const void *ptr)
                {
                    for (size_t i=0; i<nSize; ++i)
                    {
                        if (pData[i] == ptr)
                            return i;
                    }
                    return -1;
                }

                template <class T>
                    T **array(size_t offset = 0)    {
                        union {
                            void   **from;
                            T      **to;
                        } x;
                        x.from  = &pData[offset];
                        return x.to;
                    }

                template <class T>
                    T *at(size_t index)             { return reinterpret_cast<T *>(pData[index]);       }

                inline size_t size() const          { return nSize;         }

                inline size_t capacity() const      { return nCapacity;     }

                inline bool is_empty() const        { return nSize <= 0;    }

                inline void clear()
                {
                    if (pData != NULL)
                    {
                        ::free(pData);
                        pData = NULL;
                    }
                    nSize       = 0;
                    nCapacity   = 0;
                }

            public:
                explicit inline dynarray_t()
                {
                    nSize       = 0;
                    nCapacity   = 0;
                    pData       = NULL;
                }

                ~dynarray_t()
                {
                    clear();
                }
        } dynarray_t;

    }
}

#endif /* LSP_PLUG_IN_TEST_FW_DYNARRAY_H_ */
