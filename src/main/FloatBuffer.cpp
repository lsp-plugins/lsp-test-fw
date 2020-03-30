/*
 * FloatBuffer.cpp
 *
 *  Created on: 21 авг. 2018 г.
 *      Author: vsadovnikov
 */

#include <stdarg.h>
#include <string.h>

#include <lsp-plug.in/test-fw/FloatBuffer.h>

#define CK_HEAD_SIGNATURE       0xa5c33c5aL
#define CK_TAIL_SIGNATURE       0x35caac53L

namespace lsp
{
    namespace test
    {
        FloatBuffer::FloatBuffer(size_t samples, size_t align, bool aligned)
        {
            allocate(samples, align, aligned);
            randomize();
        }

        FloatBuffer::FloatBuffer(const FloatBuffer &src)
        {
            allocate(src.nLength, src.nAlign, src.bAligned);
            ::memcpy(pBuffer, src.pBuffer, src.nLength * sizeof(float));
        }

        FloatBuffer::~FloatBuffer()
        {
            if (pData != NULL)
            {
                delete  [] pData;
                pData = NULL;
            }
            pBuffer = NULL;
        }

        void FloatBuffer::allocate(size_t samples, size_t align, bool aligned)
        {
            size_t alloc    = sizeof(uint32_t)*2 + (samples + 1)*sizeof(float) + align;
            nAlign          = align;
            nLength         = samples;
            pData           = new uint8_t[alloc];
            bAligned        = aligned;
            nLastDiff       = -1;

            uint8_t *head   = &pData[sizeof(uint32_t)];
            if (aligned)
                head            = align_pointer<uint8_t>(head, nAlign);
            else if (check_alignment(head, nAlign))
                head       += sizeof(float);

            pBuffer         = reinterpret_cast<float *>(head);

            // Mark the head and the tail of the buffer with signatures
            uint32_t key    = uint32_t(ptrdiff_t(this));
            uint32_t *ptr   = reinterpret_cast<uint32_t *>(&head[-sizeof(uint32_t)]);
            *ptr            = uint32_t(CK_HEAD_SIGNATURE ^ key);
            ptr             = reinterpret_cast<uint32_t *>(&head[nLength*sizeof(float)]);
            *ptr            = uint32_t(CK_TAIL_SIGNATURE ^ key);
        }

        void FloatBuffer::fill_zero()
        {
            for (size_t i=0; i<nLength; ++i)
                pBuffer[i] = 0.0f;
        }

        bool FloatBuffer::validate() const
        {
            uint32_t key            = uint32_t(ptrdiff_t(this));
            uint8_t *head           = reinterpret_cast<uint8_t *>(pBuffer);
            uint32_t *ptr           = reinterpret_cast<uint32_t *>(&head[-sizeof(uint32_t)]);
            if (*ptr != (CK_HEAD_SIGNATURE ^ key))
                return false;
            ptr                     = reinterpret_cast<uint32_t *>(&head[nLength * sizeof(float)]);
            return (*ptr == (CK_TAIL_SIGNATURE ^ key));
        }

        bool FloatBuffer::equals_relative(const FloatBuffer &src, float tolerance) const
        {
            if (src.nLength != nLength)
                return false;
            if (!(validate() && src.validate()))
                return false;
            const float *a = pBuffer, *b = src.pBuffer;
            for (size_t i=0; i<nLength; ++i)
            {
                if (!float_equals_relative(a[i], b[i], tolerance))
                {
                    nLastDiff = i;
                    src.nLastDiff = i;
                    return false;
                }
            }
            return true;
        }

        bool FloatBuffer::equals_absolute(const FloatBuffer &src, float tolerance) const
        {
            if (src.nLength != nLength)
                return false;
            if (!(validate() && src.validate()))
                return false;
            const float *a = pBuffer, *b = src.pBuffer;
            for (size_t i=0; i<nLength; ++i)
            {
                if (!float_equals_absolute(a[i], b[i], tolerance))
                {
                    nLastDiff = i;
                    src.nLastDiff = i;
                    return false;
                }
            }
            return true;
        }

        bool FloatBuffer::equals_adaptive(const FloatBuffer &src, float tolerance) const
        {
            if (src.nLength != nLength)
                return false;
            if (!(validate() && src.validate()))
                return false;
            const float *a = pBuffer, *b = src.pBuffer;
            for (size_t i=0; i<nLength; ++i)
            {
                if (!float_equals_adaptive(a[i], b[i], tolerance))
                {
                    nLastDiff = i;
                    src.nLastDiff = i;
                    return false;
                }
            }
            return true;
        }

        void FloatBuffer::dump(FILE *out, const char *text) const
        {
            ::fprintf(out, "%s: ", text);
            for (size_t i=0; i<nLength; ++i)
                ::fprintf(out, "%.5f ", pBuffer[i]);
            ::fprintf(out, "\n");
            ::fflush(out);
        }

        void FloatBuffer::dump(FILE *out, const char *text, size_t from, size_t count) const
        {
            ::fprintf(out, "%s: ", text);
            for (size_t i=from; (i<nLength) && (count > 0); ++i, --count)
                ::fprintf(out, "%.5f ", pBuffer[i]);
            ::fprintf(out, "\n");
            ::fflush(out);
        }

        void FloatBuffer::dump_hex(FILE *out, const char *text) const
        {
            ::fprintf(out, "%s: ", text);
            const uint32_t *ptr = reinterpret_cast<const uint32_t *>(pBuffer);
            for (size_t i=0; i<nLength; ++i)
                ::fprintf(out, "%08x ", int(ptr[i]));
            ::fprintf(out, "\n");
            ::fflush(out);
        }

        void FloatBuffer::dump_hex(FILE *out, const char *text, size_t from, size_t count) const
        {
            ::fprintf(out, "%s: ", text);
            const uint32_t *ptr = reinterpret_cast<const uint32_t *>(pBuffer);
            for (size_t i=from; (i<nLength) && (count > 0); ++i, --count)
                ::fprintf(out, "%08x ", int(ptr[i]));
            ::fprintf(out, "\n");
            ::fflush(out);
        }

        void FloatBuffer::copy(const FloatBuffer &buf)
        {
            size_t to_copy = (buf.nLength < nLength) ? buf.nLength : nLength;
            ::memcpy(pBuffer, buf.pBuffer, to_copy * sizeof(float));
            while (to_copy < nLength)
                pBuffer[to_copy++] = 0.0f;
        }

        void FloatBuffer::copy(const float *buf, size_t count)
        {
            size_t to_copy = (count < nLength) ? count : nLength;
            ::memcpy(pBuffer, buf, to_copy * sizeof(float));
            while (to_copy < nLength)
                pBuffer[to_copy++] = 0.0f;
        }

        void FloatBuffer::replace(const float *buf, size_t count)
        {
            size_t to_copy = (count < nLength) ? count : nLength;
            ::memcpy(pBuffer, buf, to_copy * sizeof(float));
        }

        void FloatBuffer::vfill(size_t off, size_t n, ...)
        {
            va_list vl;
            va_start(vl, n);
            for (size_t i=off; (n > 0) && i<nLength; ++i, --n)
                pBuffer[i]  = va_arg(vl, double);
            va_end(vl);
        }
    }
}
