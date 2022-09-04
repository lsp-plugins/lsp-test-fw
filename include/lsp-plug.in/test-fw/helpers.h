/*
 * Copyright (C) 2020 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2020 Vladimir Sadovnikov <sadko4u@gmail.com>
 *
 * This file is part of lsp-test-fw
 * Created on: 27 авг. 2018 г.
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

#ifndef LSP_PLUG_IN_TEST_FW_HELPERS_H_
#define LSP_PLUG_IN_TEST_FW_HELPERS_H_

#include <lsp-plug.in/test-fw/version.h>
#include <lsp-plug.in/test-fw/types.h>

#include <stdio.h>

namespace lsp
{
    namespace test
    {
        LSP_TEST_FW_PUBLIC
        bool float_equals_relative(float a, float b, float tolerance=1e-6);

        LSP_TEST_FW_PUBLIC
        bool float_equals_absolute(float a, float b, float tolerance=1e-6);

        LSP_TEST_FW_PUBLIC
        bool float_equals_adaptive(float a, float b, float tolerance=1e-6);

        LSP_TEST_FW_PUBLIC
        void dump_bytes(FILE *out, const char *text, const void *buf, size_t count);

        LSP_TEST_FW_PUBLIC
        void dump_buffer(FILE *out, const char *text, const size_t *buf, size_t count);

        LSP_TEST_FW_PUBLIC
        void dump_buffer(FILE *out, const char *text, const ssize_t *buf, size_t count);

        LSP_TEST_FW_PUBLIC
        void dump_buffer(FILE *out, const char *text, const float *buf, size_t count);

        LSP_TEST_FW_PUBLIC
        void printf_buffer(FILE *out, const float *buf, size_t count, const char *fmt, ...);

        LSP_TEST_FW_PUBLIC
        void dump_bytes(const char *text, const void *buf, size_t count);

        LSP_TEST_FW_PUBLIC
        void dump_buffer(const char *text, const size_t *buf, size_t count);

        LSP_TEST_FW_PUBLIC
        void dump_buffer(const char *text, const ssize_t *buf, size_t count);

        LSP_TEST_FW_PUBLIC
        void dump_buffer(const char *text, const float *buf, size_t count);

        LSP_TEST_FW_PUBLIC
        void printf_buffer(const float *buf, size_t count, const char *fmt, ...);

        LSP_TEST_FW_PUBLIC
        float randf(float min, float max);

        LSP_TEST_FW_PUBLIC
        void randomize_positive(float *buf, size_t n);

        LSP_TEST_FW_PUBLIC
        void randomize_0to1(float *buf, size_t n);

        LSP_TEST_FW_PUBLIC
        void randomize(float *buf, size_t n, float min, float max);

        LSP_TEST_FW_PUBLIC
        void randomize_negative(float *buf, size_t n);

        LSP_TEST_FW_PUBLIC
        void randomize_sign(float *buf, size_t n);
    }
}

#endif /* LSP_PLUG_IN_TEST_FW_HELPERS_H_ */
