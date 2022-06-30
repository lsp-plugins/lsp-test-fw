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

#include <lsp-plug.in/test-fw/init.h>
#include <lsp-plug.in/test-fw/main/executor.h>
#include <lsp-plug.in/test-fw/main/tools.h>
#include <lsp-plug.in/test-fw/types.h>

#include <errno.h>
#include <stddef.h>
#include <stdint.h>

#if defined(LSP_TEST_FW_PLATFORM_LINUX) && defined(USE_GLIBC)
    #include <mcheck.h>
#endif /* PLATFORM_LINUX */

#ifdef LSP_TEST_FW_PLATFORM_UNIX_COMPATIBLE
    #include <signal.h>
    #include <sys/time.h>
    #include <sys/types.h>
    #include <sys/wait.h>
#endif /* LSP_TEST_FW_PLATFORM_UNIX_COMPATIBLE */

namespace lsp
{
    namespace test
    {
        TestExecutor::~TestExecutor()
        {
            wait();

            if (vTasks != NULL)
            {
                delete [] vTasks;
                vTasks      = NULL;
            }
        }

        test_status_t TestExecutor::init(config_t *config, stats_t *stats, dynarray_t *inits)
        {
        #ifdef LSP_TEST_FW_PLATFORM_WINDOWS
            SetErrorMode(SEM_NOGPFAULTERRORBOX | SEM_FAILCRITICALERRORS);
        #endif /* LSP_TEST_FW_PLATFORM_WINDOWS */

            if (config->fork)
            {
                size_t threads  = (config->threads > 0) ? config->threads : 1;
                if (config->mode != UTEST)
                    threads         = 1;

                task_t *tasks   = new task_t[threads];
                if (tasks == NULL)
                    return LSP_TEST_FW_NO_MEM;

                nTasksMax       = threads;
                nTasksActive    = 0;
                vTasks          = tasks;
            }
            else
            {
                nTasksMax       = 0;
                nTasksActive    = 0;
                vTasks          = NULL;
            }

            pCfg        = config;
            pStats      = stats;
            pInits      = inits;

            return LSP_TEST_FW_OK;
        }

        test_status_t TestExecutor::wait()
        {
            if (pCfg->is_child)
                return LSP_TEST_FW_OK;

            while (nTasksActive > 0)
            {
                test_status_t res   = wait_for_children();
                if (res != LSP_TEST_FW_OK)
                    return res;
            }

            return LSP_TEST_FW_OK;
        }

        test_status_t TestExecutor::submit(test::Test *test)
        {
            const char *tclass  = (pCfg->mode == UTEST) ? "unit test" :
                                  (pCfg->mode == PTEST) ? "performance test" :
                                  "manual test";

            // Do we need to fork() ?
            if ((!pCfg->fork) || (vTasks == NULL) || (nTasksMax <= 0))
            {
                test_status_t res = launch_test(test);
                if (pStats != NULL)
                {
                    if (res == LSP_TEST_FW_OK)
                        pStats->success.add(test);
                    else
                        pStats->failed.add(test);
                }
                return res;
            }

            // Wait for empty task descriptor
            while (nTasksActive >= nTasksMax)
            {
                test_status_t res   = wait_for_children();
                if (res != LSP_TEST_FW_OK)
                    return res;
            }

            if (!pCfg->is_child)
            {
                printf("\n--------------------------------------------------------------------------------\n");
                printf("Launching %s '%s'\n", tclass, test->full_name());
                printf("--------------------------------------------------------------------------------\n");
            }

            fflush(stdout);
            fflush(stderr);

            // Allocate new task descriptor
            task_t *task        = &vTasks[nTasksActive++];
            get_test_time(&task->submitted); // Remember start time of the test
            task->test          = test;
            task->result        = LSP_TEST_FW_OK;

            // Launch the nested process
            test_status_t res   = submit_task(task);
            if (res != LSP_TEST_FW_OK)
                --nTasksActive;
            return res;
        }

        test_status_t TestExecutor::wait_for_children()
        {
            test_clock_t ts;
            const char *test    = (pCfg->mode == UTEST) ? "Unit test" :
                                  (pCfg->mode == PTEST) ? "Performance test" :
                                  "Manual test";

            // Try to wait for child task
            task_t *task        = NULL;
            test_status_t res   = wait_for_child(&task);
            if ((res != LSP_TEST_FW_OK) || (task == NULL))
                return res;

            // Get execution time
            get_test_time(&ts);
            double time = calc_test_time_difference(&task->submitted, &ts);
            printf("%s '%s' has %s, execution time: %.2f s\n",
                    test, task->test->full_name(), (task->result == 0) ? "succeeded" : "failed", time);

            // Update statistics
            if (pStats != NULL)
            {
                if (task->result == LSP_TEST_FW_OK)
                    pStats->success.add(task->test);
                else
                    pStats->failed.add(task->test);
            }

            // Free task descriptor
            if (task < &vTasks[--nTasksActive])
                *task   = vTasks[nTasksActive];

            return LSP_TEST_FW_OK;
        }

        test_status_t TestExecutor::launch(test::UnitTest *test)
        {
            // Set-up timer for deadline expiration
            test_status_t res = LSP_TEST_FW_OK;
            if (!pCfg->debug)
                res = set_timeout(test->time_limit());

            // Launch unit test
            if (res == LSP_TEST_FW_OK)
            {
                config_t *cfg = const_cast<config_t *>(pCfg);

                test->configure(pCfg);
                start_memcheck(test);
                test->init();
                test->execute(pCfg->args.size(), cfg->args.array<const char>());
                test->destroy();
                end_memcheck();
            }

            // Cancel and disable timer
            if ((res == LSP_TEST_FW_OK) && (!pCfg->debug))
            {
                test_status_t res = kill_timeout();
                if (res != LSP_TEST_FW_OK)
                    return res;
            }

            // Return success
            return LSP_TEST_FW_OK;
        }

        test_status_t TestExecutor::launch(test::PerfTest *test)
        {
            config_t *cfg = const_cast<config_t *>(pCfg);

            // Execute performance test
            test->configure(pCfg);
            start_memcheck(test);
            test->init();
            test->execute(pCfg->args.size(), cfg->args.array<const char>());
            test->destroy();
            end_memcheck();

            // Output peformance test statistics
            printf("\nStatistics of performance test '%s':\n", test->full_name());
            test->dump_stats(stdout);

            // Additionally dump performance statistics to output file
            if (pCfg->outfile != NULL)
            {
                FILE *fd = fopen(pCfg->outfile, "a");
                if (fd != NULL)
                {
                    fprintf(fd, "--------------------------------------------------------------------------------\n");
                    fprintf(fd, "Statistics of performance test '%s':\n\n", test->full_name());
                    test->dump_stats(fd);
                    fprintf(fd, "\n");
                    fflush(fd);
                    fclose(fd);
                }
            }

            test->free_stats();

            return LSP_TEST_FW_OK;
        }

        test_status_t TestExecutor::launch(test::ManualTest *test)
        {
            config_t *cfg = const_cast<config_t *>(pCfg);
            // Execute performance test
            test->configure(pCfg);
            start_memcheck(test);
            test->init();
            test->execute(pCfg->args.size(), cfg->args.array<const char>());
            test->destroy();
            end_memcheck();

            return LSP_TEST_FW_OK;
        }

        test_status_t TestExecutor::launch_test(test::Test *test)
        {
            // 'before' should be called in direct order
            if (pInits != NULL)
            {
                for (size_t i=0, n=pInits->size(); i<n; ++i)
                {
                    test::Initializer *init = pInits->at<test::Initializer>(i);
                    init->before(test->full_name(), pCfg->mode);
                }
            }

            test_status_t result;
            switch (pCfg->mode)
            {
                case UTEST:
                    result = launch(static_cast<test::UnitTest *>(test));
                    break;
                case PTEST:
                    result = launch(static_cast<test::PerfTest *>(test));
                    break;
                case MTEST:
                    result = launch(static_cast<test::ManualTest *>(test));
                    break;
                default:
                    result = LSP_TEST_FW_BAD_STATE;
                    break;
            }

            // 'after' should be called in reverse order
            for (ssize_t i=pInits->size()-1; i>=0; --i)
            {
                test::Initializer *init = pInits->at<test::Initializer>(i);
                init->after(test->full_name(), pCfg->mode);
            }

            return result;
        }

    #ifdef LSP_TEST_FW_PLATFORM_WINDOWS
        DWORD WINAPI TestExecutor::thread_proc(LPVOID params)
        {
            TestExecutor *_this = reinterpret_cast<TestExecutor *>(params);

            HANDLE wait[2];
            wait[0] = _this->hLock;
            wait[1] = _this->hTimer;

            DWORD res   = WaitForMultipleObjects(2, wait, FALSE, INFINITE);
            // Check wait result
            if (res == WAIT_FAILED)
            {
                fprintf(stderr, "Test execution wait failed: error_code=%d\n", int(GetLastError()));
                fflush(stdout);
                fflush(stderr);
                ExitProcess(LSP_TEST_FW_TIMED_OUT);
            }
            else if (res >= WAIT_ABANDONED_0)
                res    -= WAIT_ABANDONED_0;

            if (res != 0) // Mutex object has triggered?
            {
                fprintf(stderr, "Test has timed out\n");
                fflush(stdout);
                fflush(stderr);
                ExitProcess(LSP_TEST_FW_TIMED_OUT);
            }

            return LSP_TEST_FW_OK;
        }

        test_status_t TestExecutor::set_timeout(double timeout)
        {
            hLock   = CreateMutexW(NULL, TRUE, L"timeout_handler");
            if (!hLock)
                return LSP_TEST_FW_UNKNOWN_ERR;

            hTimer  = CreateWaitableTimerW(NULL, TRUE, L"timeout_countdown");
            if (!hTimer)
            {
                CloseHandle(hLock);
                return LSP_TEST_FW_UNKNOWN_ERR;
            }

            LARGE_INTEGER liDueTime;
            liDueTime.QuadPart  = int64_t(-1e+7 * timeout); // 100 nsec intervals
            if (!SetWaitableTimer(hTimer, &liDueTime, 0, NULL, NULL, 0))
            {
                CloseHandle(hTimer);
                CloseHandle(hLock);
                return LSP_TEST_FW_UNKNOWN_ERR;
            }

            DWORD tid;
            hThread = CreateThread(NULL, 0, thread_proc, this, 0, &tid);
            if (!hThread)
            {
                CloseHandle(hTimer);
                CloseHandle(hLock);
                return LSP_TEST_FW_UNKNOWN_ERR;
            }

            return LSP_TEST_FW_OK;
        }

        test_status_t TestExecutor::kill_timeout()
        {
            ReleaseMutex(hLock);
            WaitForSingleObject(hThread, INFINITE);
            CloseHandle(hThread);
            CloseHandle(hTimer);
            CloseHandle(hLock);

            hThread     = 0;
            hTimer      = 0;
            hLock       = 0;

            return LSP_TEST_FW_OK;
        }

        test_status_t TestExecutor::submit_task(task_t *task)
        {
            test_status_t res;
            char *cmdbuf    = NULL;
            size_t len      = 0;
            size_t cap      = 0;

            // Append executable name
            res     = cmdline_append_escaped(&cmdbuf, &len, &cap, pCfg->executable, false);
            if (res != LSP_TEST_FW_OK)
                return res;

            // Append parameters
            res     = cmdline_append_escaped(&cmdbuf, &len, &cap,
                    (pCfg->mode == UTEST) ? "utest" :
                    (pCfg->mode == PTEST) ? "ptest" :
                    "mtest"
                );
            if (res == LSP_TEST_FW_OK)
                res     = cmdline_append_escaped(&cmdbuf, &len, &cap, "--run-as-nested-process");
            if (res == LSP_TEST_FW_OK)
                res     = cmdline_append_escaped(&cmdbuf, &len, &cap, "-nsi");
            if (res == LSP_TEST_FW_OK)
                res     = cmdline_append_escaped(&cmdbuf, &len, &cap, "-nf");
            if ((res == LSP_TEST_FW_OK) && (pCfg->debug))
                res     = cmdline_append_escaped(&cmdbuf, &len, &cap, "-d");
            if (res == LSP_TEST_FW_OK)
                res     = cmdline_append_escaped(&cmdbuf, &len, &cap, (pCfg->verbose) ? "-v" : "-s");
            if ((res == LSP_TEST_FW_OK) && (pCfg->outfile != NULL))
            {
                res     = cmdline_append_escaped(&cmdbuf, &len, &cap, "-o");
                if (res == LSP_TEST_FW_OK)
                    res     = cmdline_append_escaped(&cmdbuf, &len, &cap, pCfg->outfile);
            }
            if ((res == LSP_TEST_FW_OK) && (pCfg->tracepath != NULL))
            {
                res     = cmdline_append_escaped(&cmdbuf, &len, &cap, "-t");
                if (res == LSP_TEST_FW_OK)
                    res     = cmdline_append_escaped(&cmdbuf, &len, &cap, pCfg->tracepath);
            }
            if ((res == LSP_TEST_FW_OK) && (pCfg->tempdir != NULL))
            {
                res     = cmdline_append_escaped(&cmdbuf, &len, &cap, "-td");
                if (res == LSP_TEST_FW_OK)
                    res     = cmdline_append_escaped(&cmdbuf, &len, &cap, pCfg->tempdir);
            }
            if ((res == LSP_TEST_FW_OK) && (pCfg->resource != NULL))
            {
                res     = cmdline_append_escaped(&cmdbuf, &len, &cap, "-r");
                if (res == LSP_TEST_FW_OK)
                    res     = cmdline_append_escaped(&cmdbuf, &len, &cap, pCfg->resource);
            }
            if (res == LSP_TEST_FW_OK)
                res     = cmdline_append_escaped(&cmdbuf, &len, &cap, task->test->full_name());
            if ((res == LSP_TEST_FW_OK) && (pCfg->args.size() > 0))
            {
                res     = cmdline_append_escaped(&cmdbuf, &len, &cap, "-a");
                if (res == LSP_TEST_FW_OK)
                {
                    for (size_t i=0, n=pCfg->args.size(); i<n; ++i)
                        if ((res = cmdline_append_escaped(&cmdbuf, &len, &cap, pCfg->args.at<char>(i))) != LSP_TEST_FW_OK)
                            break;
                }
            }

            if (res != LSP_TEST_FW_OK)
            {
                free(cmdbuf);
                return res;
            }

            // Allocate arguments and executable strings
            WCHAR *cmd          = utf8_to_utf16(cmdbuf);
            free(cmdbuf);
            if (cmd == NULL)
                return LSP_TEST_FW_NO_MEM;
            WCHAR *executable   = utf8_to_utf16(pCfg->executable);
            if (executable == NULL)
            {
                free(cmd);
                return LSP_TEST_FW_NO_MEM;
            }

            // Launch child process
            STARTUPINFOW si;
            PROCESS_INFORMATION pi;

            ZeroMemory( &si, sizeof(STARTUPINFOW) );
            ZeroMemory( &pi, sizeof(PROCESS_INFORMATION) );
            si.cb = sizeof(si);

            // Start the child process.
            if( !CreateProcessW(
                executable,     // No module name (use command line)
                cmd,            // Command line
                NULL,           // Process handle not inheritable
                NULL,           // Thread handle not inheritable
                FALSE,          // Set handle inheritance to FALSE
                0,              // No creation flags
                NULL,           // Use parent's environment block
                NULL,           // Use parent's starting directory
                &si,            // Pointer to STARTUPINFO structure
                &pi             // Pointer to PROCESS_INFORMATION structure
            ))
            {
                fprintf(stderr, "Failed to create child process (%d)\n", int(GetLastError()));
                fflush(stderr);

                WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), cmd, len, NULL, NULL);
                free(cmd);
                free(executable);
                return LSP_TEST_FW_UNKNOWN_ERR;
            }

            free(cmd);
            free(executable);

            task->pid       = pi;
            return LSP_TEST_FW_OK;
        }

        test_status_t TestExecutor::wait_for_child(task_t **task)
        {
            if (nTasksActive <= 0)
                return LSP_TEST_FW_NOT_FOUND;

            // Wait for any child process exit event
            HANDLE *pids    = reinterpret_cast<HANDLE *>(alloca(nTasksActive * sizeof(HANDLE)));
            for (size_t i=0; i<nTasksActive; ++i)
                pids[i]         = vTasks[i].pid.hProcess;

            size_t idx      = WaitForMultipleObjects(nTasksActive, pids, FALSE, INFINITE);

            // Get termination code
            DWORD code;
            GetExitCodeProcess(pids[idx], &code);
            vTasks[idx].result  = code;
            *task       = &vTasks[idx];

            return LSP_TEST_FW_OK;
        }
    #endif /* LSP_TEST_FW_PLATFORM_WINDOWS */

    #if defined(LSP_TEST_FW_PLATFORM_LINUX) && defined(USE_GLIBC)
        void TestExecutor::start_memcheck(test::Test *v)
        {
            if (!pCfg->mtrace)
                return;

            // Enable memory trace
            char fname[PATH_MAX];
            mkdirs(pCfg->tracepath);
            snprintf(fname, PATH_MAX, "%s/%s.utest.mtrace", pCfg->tracepath, v->full_name());
            fname[PATH_MAX-1] = '\0';

            fprintf(stderr, "Enabling memory trace for test '%s' into file '%s'\n", v->full_name(), fname);
            fflush(stderr);

            setenv("MALLOC_TRACE", fname, 1);

            mtrace();
        }

        void TestExecutor::end_memcheck()
        {
            // Disable memory trace
            if (pCfg->mtrace)
                muntrace();

            // Validate heap
    //        mcheck_check_all();
        }
    #else
        void TestExecutor::start_memcheck(test::Test *v)
        {
        }

        void TestExecutor::end_memcheck()
        {
        }
    #endif /* LSP_TEST_FW_PLATFORM_LINUX */

    #ifdef LSP_TEST_FW_PLATFORM_UNIX_COMPATIBLE
        void utest_timeout_handler(int signum)
        {
            fprintf(stderr, "Unit test time limit exceeded\n");
            exit(LSP_TEST_FW_TIMED_OUT);
        }

        test_status_t TestExecutor::submit_task(task_t *task)
        {
            pid_t pid = fork();
            if (pid == 0)
            {
                pCfg->is_child  = true;
                return launch_test(task->test);
            }
            else if (pid < 0)
            {
                int error = errno;
                fprintf(stderr, "Error while spawning child process %d\n", error);
                fflush(stderr);
                return LSP_TEST_FW_UNKNOWN_ERR;
            }

            task->pid       = pid;
            return LSP_TEST_FW_OK;
        }

        test_status_t TestExecutor::wait_for_child(task_t **task)
        {
            task_t *ret       = NULL;
            int result;

            do
            {
                pid_t pid = ::waitpid(-1, &result, WUNTRACED | WCONTINUED);
                if (pid < 0)
                {
                    fprintf(stderr, "Child process completion wait failed\n");
                    return LSP_TEST_FW_UNKNOWN_ERR;
                }

                // Find the associated thread process
                ret       = NULL;
                for (size_t i=0; i<nTasksActive; ++i)
                    if (vTasks[i].pid == pid)
                    {
                        ret   = &vTasks[i];
                        break;
                    }

                if (WIFSTOPPED(result))
                    printf("Child process %d stopped by signal %d\n", int(pid), WSTOPSIG(result));
            } while ((!WIFEXITED(result)) && !WIFSIGNALED(result));

            if (WIFEXITED(result))
                ret->result   = WEXITSTATUS(result);
            else if (WIFSIGNALED(result))
                ret->result   = LSP_TEST_FW_KILLED;

            *task   = ret;
            return LSP_TEST_FW_OK;
        }

        test_status_t TestExecutor::set_timeout(double timeout)
        {
            struct itimerval timer;

            timer.it_interval.tv_sec    = timeout;
            timer.it_interval.tv_usec   = suseconds_t(timeout * 1e+6) % 1000000L;
            timer.it_value              = timer.it_interval;

            test_status_t res           = LSP_TEST_FW_OK;
            if (setitimer(ITIMER_REAL, &timer, NULL) != 0)
            {
                int code = errno;
                fprintf(stderr, "setitimer failed with errno=%d\n", code);
                fflush(stderr);
                res = LSP_TEST_FW_UNKNOWN_ERR;
            }
            signal(SIGALRM, utest_timeout_handler);

            return res;
        }

        test_status_t TestExecutor::kill_timeout()
        {
            struct itimerval timer;

            timer.it_interval.tv_sec    = 0;
            timer.it_interval.tv_usec   = 0;
            timer.it_value              = timer.it_interval;

            signal(SIGALRM, SIG_DFL);
            if (setitimer(ITIMER_REAL, &timer, NULL) == 0)
                return LSP_TEST_FW_OK;

            int code = errno;
            fprintf(stderr, "setitimer failed with errno=%d\n", code);
            fflush(stderr);
            return LSP_TEST_FW_UNKNOWN_ERR;
        }

    #endif /* LSP_TEST_FW_PLATFORM_UNIX_COMPATIBLE */
    }
}


