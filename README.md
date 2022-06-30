# lsp-test-fw

This is test framework for executing automated and manual tests.

It allows to write the following set of tests
* Unit tests - automated short-time tests for checking validity of algorithms
* Performance tests - automated single-core tests for checking and comparing the
performance of different implementations of the same algorithm or how different
parameters do affect the execution of the same algorithm.
* Manual tests - tests that can not be automated but can be simly launched and
performed 'by hand'.

Supported platforms
======

The build and correct unit test execution has been confirmed for following platforms:
* FreeBSD
* GNU/Linux
* OpenBSD
* Windows 32-bit
* Windows 64-bit

Requirements
======

The following packages need to be installed for building:

* gcc >= 4.9
* make >= 4.0

Building
======

To build the library, perform the following commands:

```bash
make config # Configure the build
make fetch # Fetch dependencies from Git repository
make
sudo make install
```

To get more build options, run:

```bash
make help
```

To uninstall library, simply issue:

```bash
make uninstall
```

To clean all binary files, run:

```bash
make clean
```

To clean the whole project tree including configuration files, run:

```bash
make prune
```

Integrating
======

To compile compile the object file for future linking, perform the following commands:
```bash
make config # Configure the build
make fetch # Fetch dependencies from Git repository
make compile
```
These commands will create the ```.build/lsp-test-fw/lsp-test-fw.o``` file by default
which can be linked to your project.

To get more options, run:

```bash
make help
```

To build framework with demo tests, issue the following command:
```bash
make config TEST=1 # Configure the build
make fetch # Fetch dependencies from Git repository
make compile
```

Launching
======

To build application which will allow to execute tests, issue the following command:
```bash
make config TEST=1 # Configure the build
make fetch # Fetch dependencies from Git repository
make
```

These commands will create the ```.build/lsp-test-fw/lsp-test-fw-test``` executable.

To get more options, run:

```bash
make help
```

Manual tests are semi-automated or not automated at all. You may launch these tests
to perform manual interaction with system or for retrieving some text data for further
processing.

Unit tests are completely automated and check that the produced result is correct for the
specific input data. By default unit tests are launched in separate processes and utilize
all CPU cores for maximum performance. The status of each unit test is collected during
execution and list of all failed tests are printed after completion.

Performance tests measure performance of some function/module under different conditions, 
gather final statistics and output them in a table format. These tests are very useful for
measuring single-core performance of different modules and functions and performing code
optimizations.

After build, we can launch the test binary by issuing command:

```bash
.build/lsp-test-fw/lsp-test-fw-test
```

The executable file provides simple command-line interface, so here's the full usage:

```
USAGE: {utest|ptest|mtest} [args...] [test name...]
  First argument:
    utest                 Unit testing subsystem
    ptest                 Performance testing subsystem
    mtest                 Manual testing subsystem
  Additional arguments:
    -a, --args [args...]  Pass arguments to test
    -d, --debug           Disable time restrictions for unit tests
                          for debugging purposes
    -e, --execute         Launch tests specified after this switch
    -f, --fork            Fork child processes (opposite to --nofork)
    -h, --help            Display help
    -i, --ignore          Ignore tests specified after this switch
    -j, --jobs            Set number of job workers for unit tests
    -l, --list            List all available tests
    -mt, --mtrace         Enable mtrace log
    -nf, --nofork         Do not fork child processes (for better 
                          debugging capabilities)
    -nt, --nomtrace       Disable mtrace log
    -nsi, --nosysinfo     Do not output system information
    -o, --outfile file    Output performance test statistics to specified file
    -r, --resource path   Location of the resource directory used by tests,
                          default resource path is 'res/test'
    -s, --silent          Do not output additional information from tests
    -ss, --suppress       Suppress extra messages, output only necessary information
    -si, --sysinfo        Output system information
    -t, --tracepath path  Override default trace path with specified value
                          default trace path is '/tmp/lsp-test-trace'
    -td, --tempdir path   Override default temporary directory for tests,
                          default temporary directory is '/tmp/lsp-test-temp'
    -v, --verbose         Output additional information from tests
```

Each test has fully-qualified name separated by dot symbols, tests from different
test spaces (utest, ptest, mtest) may have similar fully-qualified names. Test names
are case-sensitive.

To obtain a list of all unit tests we can issue:

```bash
  .build/lsp-test-fw/lsp-test-fw-test utest --list
```

And then it is possible to launch all demo tests:

```bash
  .test/lsp-plugins-test utest demo.*
```

If there is no unit test specified in arguments, all available unit tests
will be launched.

To start unit test in debug mode, additional parameters are required to be passed:

```bash
  .test/lsp-plugins-test utest --nofork --debug --verbose
```
  
Because unit tests are short-time fully-automated tests, they are parallelized and
executed by default by number_of_cores*2 processes. To disable this, option ```--nofork``` is used.
Also, unit test execution time is limited by 5 seconds by default, so when the test gets stuck on
a break point, it gets killed. To prevent this, additional option ```--debug``` is required to be passed.
Option ```--verbose``` allows to the test to output additional information and is turned off by default.

Performance tests can be used to obtain full performance profile of target machine.
Because performance tests in most cases take much time for gathering statistics,
the final statistics for each test can be saved in a separate file by specifying ```--outfile```
option:

```bash
  .test/lsp-plugins-test ptest -o performance-test.log
```

Manual tests are mostly designed for developers' purposes.
