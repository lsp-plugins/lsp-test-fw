/*
 * env.h
 *
 *  Created on: 22 мар. 2020 г.
 *      Author: sadko
 */

#ifndef LSP_PLUG_IN_TEST_FW_ENV_H_
#define LSP_PLUG_IN_TEST_FW_ENV_H_

#include <lsp-plug.in/test-fw/main/config.h>

namespace lsp
{
    namespace test
    {
        class Environment
        {
            private:
                Environment & operator = (const Environment &);

            private:
                bool                __verbose;
                const char         *__executable;
                FILE               *__stdout;
                FILE               *__stderr;

            public:
                explicit Environment();
                virtual ~Environment();

            public:
                virtual void        configure(config_t *cfg);

            public:
                inline const char  *executable() const      { return __executable; }
                inline bool         verbose() const         { return __verbose; }

            public:
                int                 printf(const char *fmt, ...);
                int                 eprintf(const char *fmt, ...);
        };
    }
}


#endif /* LSP_PLUG_IN_TEST_FW_ENV_H_ */
