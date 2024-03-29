/*
 * Copyright (C) 2020 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2020 Vladimir Sadovnikov <sadko4u@gmail.com>
 *
 * This file is part of lsp-test-fw
 * Created on: 21 авг. 2018 г.
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

#include <stdlib.h>
#include <string.h>

#include <lsp-plug.in/test-fw/ByteBuffer.h>

#define CK_HEAD_SIGNATURE       0xc3a53c5aL
#define CK_TAIL_SIGNATURE       0xca35ac53L

namespace lsp
{
    namespace test
    {
        ByteBuffer::ByteBuffer(size_t samples, size_t align, bool aligned)
        {
            allocate(samples, align, aligned);
            randomize();
        }

        ByteBuffer::ByteBuffer(const ByteBuffer &src)
        {
            allocate(src.nLength, src.nAlign, src.bAligned);
            ::memcpy(pBuffer, src.pBuffer, src.nLength);
        }

        ByteBuffer::ByteBuffer(const void *data, size_t samples, size_t align, bool aligned)
        {
            allocate(samples, align, aligned);
            ::memcpy(pBuffer, data, samples);
        }

        ByteBuffer::~ByteBuffer()
        {
            if (pData != NULL)
            {
                delete  [] pData;
                pData = NULL;
            }
            pBuffer = NULL;
        }

        void ByteBuffer::allocate(size_t samples, size_t align, bool aligned)
        {
            size_t alloc    = sizeof(uint32_t)*2 + samples + align;
            nAlign          = align;
            nLength         = samples;
            pData           = new uint8_t[alloc];
            bAligned        = aligned;

            uint8_t *head   = &pData[sizeof(uint32_t)];
            if (aligned)
                head            = align_pointer<uint8_t>(head, nAlign);
            else if (check_alignment(head, nAlign))
                head       += sizeof(float);

            pBuffer         = head;

            // Mark the head and the tail of the buffer with signatures
            uint32_t key    = uint32_t(ptrdiff_t(this));
            uint32_t *ptr   = reinterpret_cast<uint32_t *>(&pBuffer[-sizeof(uint32_t)]);
            *ptr            = uint32_t(CK_HEAD_SIGNATURE ^ key);
            ptr             = reinterpret_cast<uint32_t *>(&pBuffer[nLength]);
            *ptr            = uint32_t(CK_TAIL_SIGNATURE ^ key);
        }

        void ByteBuffer::randomize()
        {
            for (size_t i=0; i<nLength; ++i)
                pBuffer[i] = uint8_t(::rand());
        }

        void ByteBuffer::fill_zero()
        {
        #ifdef LSP_TEST_FW_PLATFORM_WINDOWS
            ::memset(pBuffer, 0, nLength);
        #else
            ::bzero(pBuffer, nLength);
        #endif /* LSP_TEST_FW_ */
        }

        bool ByteBuffer::validate() const
        {
            uint32_t key            = uint32_t(ptrdiff_t(this));
            const uint32_t *ptr     = reinterpret_cast<uint32_t *>(&pBuffer[-sizeof(uint32_t)]);
            if (*ptr != (CK_HEAD_SIGNATURE ^ key))
                return false;
            ptr                     = reinterpret_cast<uint32_t *>(&pBuffer[nLength]);
            return (*ptr == (CK_TAIL_SIGNATURE ^ key));
        }

        bool ByteBuffer::equals(const ByteBuffer &src) const
        {
            return ::memcmp(pBuffer, src.pBuffer, nLength) == 0;
        }

        bool ByteBuffer::equals(const void *src, size_t count) const
        {
            if (count > nLength)
                return false;
            return ::memcmp(pBuffer, src, count) == 0;
        }

        bool ByteBuffer::equals(const void *src, size_t offset, size_t count) const
        {
            if ((count + offset) > nLength)
                return false;
            return ::memcmp(&pBuffer[offset], src, count) == 0;
        }

        void ByteBuffer::dump(FILE *out, const char *text) const
        {
            ::fprintf(out, "%s: ", text);
            for (size_t i=0; i<nLength; ++i)
                ::fprintf(out, "%02x ", pBuffer[i]);
            ::fprintf(out, "\n");
        }

        void ByteBuffer::dump(FILE *out, const char *text, size_t from, size_t count) const
        {
            ::fprintf(out, "%s: ", text);
            for (size_t i=0; i<nLength; ++i)
                ::fprintf(out, "%02x ", pBuffer[i]);
            ::fprintf(out, "\n");
        }
    }
}
