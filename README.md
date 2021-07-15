
# gonk

[![Build Status](https://app.travis-ci.com/strandfield/gonk.svg?branch=master)](https://app.travis-ci.com/strandfield/gonk)
[![Build status](https://ci.appveyor.com/api/projects/status/rk5adpuetotx2txv?svg=true)](https://ci.appveyor.com/project/strandfield/gonk)

`gonk` is a statically-typed scripting language written in C++17.

Its goal is not to become a fully-featured programming language but rather 
to showcase and drive the development of the [libscript](https://github.com/strandfield/libscript) 
library on which it is built.

The syntax for running scripts with the command line is:

```bash
gonk script-name arg1 arg2 ... argN
```

Scripts have the extension `.gnk`. Examples are available in the `examples` folder.

Although `gonk` primary job is to run scripts, an interactive session can also be 
started using `gonk --interactive`:

```bash
gonk --interactive
>>> import std.math
>>> a = 0.5
0.500000
>>> b = std::cos(a)
0.877583
>>> c = std::sin(a)
0.479426
>>> std::pow(b, 2) + std::pow(c, 2)
1.000000
>>> :q
```

As this interactive session shows, `gonk` provides a module system.
Use `gonk --list-modules` to list available modules.

A (very) basic debugger named `gonkdbg` is also available.
Below is a screenshot of the debugger, invoked with `gonkdbg math.gnk`.

![Screenshot of gonkdbg](docs/images/dbg.png)

## Getting started

### Pre-built binaries (Windows-only)

Download the latest pre-built binaries for Windows directly from GitHub in 
the [Releases](https://github.com/strandfield/gonk/releases) tab.

### Building

You will need CMake and Qt5 to build the project.

Please refer to `appveyor.yml` (Windows) or `.travis.yml` (Ubuntu) to get 
exact working commands to build the project. The steps are summarized as follow:

1. Clone the project

```bash
git clone https://github.com/strandfield/gonk.git --recursive
```

2. Build

Inside the repository, create a build dir and build with cmake.

```bash
mkdir build && cd build
cmake ..
make
```

3. Test

After a successful build, move to the test directory and run ctest.

```bash
cd tests
ctest
```
