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

#ifndef LSP_PLUG_IN_TEST_FW_ENV_H_
#define LSP_PLUG_IN_TEST_FW_ENV_H_

#include <lsp-plug.in/test-fw/version.h>
#include <lsp-plug.in/test-fw/main/config.h>

namespace lsp
{
    namespace test
    {
        class Environment
        {
            private:
                Environment & operator = (const Environment &);

            protected:
                bool                __verbose;
                const char         *__executable;
                const char         *__tempdir;
                const char         *__resources;
                FILE               *__stdout;
                FILE               *__stderr;

            public:
                explicit Environment();
                virtual ~Environment();

            public:
                virtual void        configure(config_t *cfg);

            public:
                inline const char  *executable() const      { return __executable;      }
                inline bool         verbose() const         { return __verbose;         }
                inline const char  *tempdir() const         { return __tempdir;         }
                inline const char  *resources() const       { return __resources;       }

            public:
                int                 printf(const char *fmt, ...);
                int                 eprintf(const char *fmt, ...);
        };
    }
}


#endif /* LSP_PLUG_IN_TEST_FW_ENV_H_ */
