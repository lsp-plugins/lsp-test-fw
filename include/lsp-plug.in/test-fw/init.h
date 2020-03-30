/*
 * init.h
 *
 *  Created on: 22 мар. 2020 г.
 *      Author: sadko
 */

#ifndef LSP_PLUG_IN_TEST_FW_INIT_H_
#define LSP_PLUG_IN_TEST_FW_INIT_H_

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
                friend status_t         init_init(dynarray_t *list);

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
                 *
                 */
                virtual void            initialize();

                /**
                 * Output additional info if required
                 */
                virtual void            info();

                /**
                 * Call a finalizer function at test end
                 */
                virtual void            finalize();
        };
    }
}

#endif /* LSP_PLUG_IN_TEST_FW_INIT_H_ */
