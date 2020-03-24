/*
 * helpers.cpp
 *
 *  Created on: 27 авг. 2018 г.
 *      Author: sadko
 */

#include <math.h>
#include <stdarg.h>

#include <lsp-plug.in/test-fw/helpers.h>

namespace lsp
{
    namespace test
    {
        bool float_equals_relative(float a, float b, float tolerance)
        {
            if (a == 0.0f)
                return (fabs(b) < tolerance);
            else if (b == 0.0f)
                return (fabs(a) < tolerance);

            float diff = (a > b) ? fabs(a/b) : fabs(b/a);
            return fabs(1.0f - diff) < tolerance;
        }

        bool float_equals_absolute(float a, float b, float tolerance)
        {
            return fabs(a - b) <= tolerance;
        }

        bool float_equals_adaptive(float a, float b, float tolerance)
        {
            return (fabs(a) > 1.0f) ?
                float_equals_relative(a, b, tolerance) :
                float_equals_absolute(a, b, tolerance);
        }

        void dump_bytes(FILE *out, const char *text, const void *buf, size_t count)
        {
            ::fprintf(out, "  dump of buffer %s[%p]:\n    ", text, buf);
            const uint8_t *b = reinterpret_cast<const uint8_t *>(buf);
            while (count--)
                ::fprintf(out, "%02x ", *(b++));
            ::fprintf(out, "\n");
            ::fflush(out);
        }

        void dump_buffer(FILE *out, const char *text, const size_t *buf, size_t count)
        {
            ::fprintf(out, "  dump of buffer %s[%p]:\n    ", text, buf);
            while (count--)
                ::fprintf(out, "%lu ", (long unsigned int)(*(buf++)));
            ::fprintf(out, "\n");
            ::fflush(out);
        }

        void dump_buffer(FILE *out, const char *text, const ssize_t *buf, size_t count)
        {
            ::fprintf(out, "  dump of buffer %s[%p]:\n    ", text, buf);
            while (count--)
                ::fprintf(out, "%ld ", (long)(*(buf++)));
            ::fprintf(out, "\n");
            ::fflush(out);
        }

        void dump_buffer(FILE *out, const char *text, const float *buf, size_t count)
        {
            ::fprintf(out, "dump of buffer %s[%p]:\n", text, buf);
            while (count--)
                ::fprintf(out, "%.30f\n", *(buf++));
            ::fflush(out);
        }

        void printf_buffer(FILE *out, const float *buf, size_t count, const char *fmt, ...)
        {
            va_list vl;
            va_start(vl, fmt);
            ::vfprintf(out, fmt, vl);
            ::fputs("\n", out);
            va_end(vl);

            for (size_t i=0; i<count; ++i)
            {
                if (i > 0)
                    ::fputc(' ', out);
                ::fprintf(out, "%.6f", buf[i]);
            }
            ::fputs("\n", out);
            ::fflush(out);
        }

        void dump_buffer(const char *text, const size_t *buf, size_t count)
        {
            dump_buffer(stdout, text, buf, count);
        }

        void dump_bytes(const char *text, const void *buf, size_t count)
        {
            dump_bytes(stdout, text, buf, count);
        }

        void dump_buffer(const char *text, const ssize_t *buf, size_t count)
        {
            dump_buffer(stdout, text, buf, count);
        }

        void dump_buffer(const char *text, const float *buf, size_t count)
        {
            dump_buffer(stdout, text, buf, count);
        }

        void printf_buffer(const float *buf, size_t count, const char *fmt, ...)
        {
            va_list vl;
            va_start(vl, fmt);
            ::vprintf(fmt, vl);
            ::puts("");
            va_end(vl);

            for (size_t i=0; i<count; ++i)
            {
                if (i > 0)
                    ::putchar(' ');
                ::printf("%.6f", buf[i]);
            }
            ::puts("");
            ::fflush(stdout);
        }

        float randf(float min, float max)
        {
            float v = float(rand()) / (float(RAND_MAX) + 1.0f);
            return min + (max - min) * v;
        }

        void randomize_positive(float *buf, size_t n)
        {
            for (size_t i=0; i<n; ++i)
                buf[i] = (float(rand())/float(RAND_MAX)) + 0.001f;
        }

        void randomize_0to1(float *buf, size_t n)
        {
            for (size_t i=0; i<n; ++i)
                buf[i] = (float(rand())/float(RAND_MAX-1));
        }

        void randomize(float *buf, size_t n, float min, float max)
        {
            float delta = max - min;
            for (size_t i=0; i<n; ++i)
                buf[i] = min + delta * (float(rand())/(RAND_MAX-1));
        }

        void randomize_negative(float *buf, size_t n)
        {
            for (size_t i=0; i<n; ++i)
                buf[i] = - ((float(rand())/float(RAND_MAX)) + 0.001f);
        }

        void randomize_sign(float *buf, size_t n)
        {
            for (size_t i=0; i<n; ++i)
            {
                float tmp = (float(rand())/float(RAND_MAX)) + 0.001f;
                buf[i] = (rand() >= (RAND_MAX >> 1)) ? tmp : -tmp;
            }
        }
    }
}


