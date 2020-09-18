/*
 * Copyright (C) 2020 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2020 Vladimir Sadovnikov <sadko4u@gmail.com>
 *
 * This file is part of lsp-test-fw
 * Created on: 11 февр. 2019 г.
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

#ifndef TEST_MAIN_EXECUTOR_H_
#define TEST_MAIN_EXECUTOR_H_

#include <lsp-plug.in/test-fw/version.h>

#include <lsp-plug.in/test-fw/utest.h>
#include <lsp-plug.in/test-fw/mtest.h>
#include <lsp-plug.in/test-fw/ptest.h>

#include <lsp-plug.in/test-fw/main/types.h>
#include <lsp-plug.in/test-fw/main/config.h>

namespace lsp
{
    namespace test
    {
        class TestExecutor
        {
            protected:
                typedef struct task_t
                {
                    test_pid_t          pid;
                    test_clock_t        submitted;
                    test::Test         *test;
                    status_t            result;
                } task_t;

            private:
                size_t              nTotal;
                size_t              nTasksMax;
                size_t              nTasksActive;
                double              fOverall;
                task_t             *vTasks;
                config_t           *pCfg;
                stats_t            *pStats;
                dynarray_t         *pInits;
    #ifdef PLATFORM_WINDOWS
                HANDLE              hThread;
                HANDLE              hTimer;
                HANDLE              hLock;
    #endif /* PLATFORM_WINDOWS */

            protected:
                status_t    launch_test(test::Test *test);
                status_t    wait_for_children();
                status_t    launch(test::UnitTest *test);
                status_t    launch(test::PerfTest *test);
                status_t    launch(test::ManualTest *test);

                // Platform-dependent routines
                status_t    submit_task(task_t *task);
                status_t    wait_for_child(task_t **task);
                status_t    set_timeout(double timeout);
                status_t    kill_timeout();
                void        start_memcheck(test::Test *test);
                void        end_memcheck();

    #ifdef PLATFORM_WINDOWS
                static DWORD WINAPI  thread_proc(LPVOID params);
    #endif /* PLATFORM_WINDOWS */

            public:
                explicit TestExecutor()
                {
                    nTotal          = 0;
                    nTasksMax       = 0;
                    nTasksActive    = 0;
                    fOverall        = 0.0f;
                    vTasks          = NULL;
                    pCfg            = NULL;
                    pStats          = NULL;
                    pInits          = NULL;

    #ifdef PLATFORM_WINDOWS
                    hThread         = 0;
                    hTimer          = 0;
                    hLock           = 0;
    #endif /* PLATFORM_WINDOWS */
                }

                ~TestExecutor();

            public:
                /**
                 * Configure test launcher
                 * @param config launcher configuration
                 * @param stats pointer to statistics structure
                 * @return status of operation
                 */
                status_t init(config_t *config, stats_t *stats, dynarray_t *inits);

                /**
                 * Wait for completion of all child processes
                 * @return  status of operation
                 */
                status_t wait();

                /**
                 * Submit test for execution
                 * @param test test for execution
                 * @return status of operation
                 */
                status_t submit(test::Test *test);
        };
    }
}


#endif /* TEST_MAIN_EXECUTOR_H_ */
