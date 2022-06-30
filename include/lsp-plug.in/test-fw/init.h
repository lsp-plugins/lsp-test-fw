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

#ifndef LSP_PLUG_IN_TEST_FW_INIT_H_
#define LSP_PLUG_IN_TEST_FW_INIT_H_

#include <lsp-plug.in/test-fw/version.h>
#include <lsp-plug.in/test-fw/env.h>
#include <lsp-plug.in/test-fw/main/dynarray.h>

#define INIT_BEGIN(name) \
        namespace lsp { \
        namespace test_init { \
        namespace { \
            \
            using namespace ::lsp::test; \
            \
            class init_ ## name: public Initializer { \
                private: \
                    init_ ## name & operator = (const init_ ## name &); \
                \
                public: \
                    typedef init_ ## name init_type_t;\
                \
                public: \
                    \
                    _Pragma("GCC diagnostic push") \
                    _Pragma("GCC diagnostic ignored \"-Wuninitialized\"") \
                    explicit init_ ## name() : Initializer(#name) {} \
                    _Pragma("GCC diagnostic pop") \
                    \
                    virtual ~init_ ## name() {}

#define INIT_FUNC   virtual void initialize()
#define INFO_FUNC   virtual void info()
#define FINI_FUNC   virtual void finalize()
#define BEFORE_FUNC virtual void before(const char *test, test_mode_t type)
#define AFTER_FUNC  virtual void after(const char *test, test_mode_t type)

#define INIT_END \
        } initializer;  /* initializer class */ \
        } /* anonymous namespace */ \
        } /* namespace test_init */ \
        } /* namespace lsp */

namespace lsp
{
    namespace test
    {
        /**
         * Test initializer base class
         */
        class Initializer: public Environment
        {
            private:
                Initializer & operator = (const Initializer &);

            private:
                friend test_status_t    init_init(dynarray_t *list);

            private:
                static Initializer     *__root;
                Initializer            *__next;
                const char             *__name;

            public:
                explicit Initializer(const char *name);
                virtual ~Initializer();

            public:
                inline const char      *full_name() const { return __name; };

            public:
                /** Call an initializer function at test start
                 * This function is executed once at process start.
                 *
                 * This function can be used to initialize global state of program
                 * before the target test will be called.
                 *
                 * IMPORTANT: Since different operating systems provide different
                 * Inter-process-communication, don't consider that this
                 * function will start only once for the whole test lifecycle.
                 *
                 */
                virtual void            initialize();

                /**
                 * Output additional info if required
                 */
                virtual void            info();

                /**
                 * Call a finalizer function at test end
                 *
                 * IMPORTANT: Since different operating systems provide different
                 * Inter-process-communication, don't consider that this
                 * function will start only once for the whole test lifecycle.
                 */
                virtual void            finalize();

                /**
                 * Call function before each test
                 * @param test fully-qualified test name
                 * @param type type of test
                 */
                virtual void            before(const char *test, test_mode_t type);

                /**
                 * Call function after each test
                 * @param test fully-qualified test name
                 * @param type type of test
                 */
                virtual void            after(const char *test, test_mode_t type);
        };
    }
}

#endif /* LSP_PLUG_IN_TEST_FW_INIT_H_ */
