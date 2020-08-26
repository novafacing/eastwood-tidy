# eastwood-tidy


## Table Of Contents
1. [About This Project](#about)
2. [Building](#building)
3. [Installation](#installation)
4. [Usage](#installation)
5. [Testing](#testing)
6. [Notes](#notes)

## About <a name="about" />

`eastwood-tidy` is a port of the Eastwood C language linter to the clang-tidy check system.

Initial work was done by Connor McMillin. The project is now maintained by Rowan Hart.

## Building <a name="building" />

1. Get the llvm github repo:

```
$ git clone https://github.com/llvm/llvm-project.git
```
    
2. Clone this repo as a submodule (if you fork llvm-project) or simple clone (if you don't). You probably want simple clone.

Submodule:

```
$ git submodule add https://github.com/novafacing/eastwood-tidy.git \
    llvm-project/clang-tools-extra/clang-tidy/eastwood
```

Simple clone:

```
$ git clone https://github.com/novafacing/eastwood-tidy.git \
    llvm-project/clang-tools-extra/clang-tidy/eastwood
```

3. Copy files from eastwood/setup to the proper places in llvm-project/clang-tools-extra/clang-tidy/

```
$ cp llvm-project/clang-tools-extra/clang-tidy/eastwood/setup/CMakeLists.txt \
    llvm-project/clang-tools-extra/clang-tidy/

$ cp llvm-project/clang-tools-extra/clang-tidy/eastwood/setup/ClangTidyForceLinker.h \
    llvm-project/clang-tools-extra/clang-tidy/
```
    
4. Use CMake + Make to build the new clang-tidy

```
cd 
mkdir build
cd build
cmake -DLLVM_ENABLE_PROJECTS='clang;clang-tools-extra' ..
make -j NN clang-tidy
```

5. Binary will be located at `llvm-project/llvm/build/bin/clang-tidy`

## Installation <a name="installation" />

The binary can be installed by simply copying it to a `$PATH` location.

## Usage <a name="usage" />

The program can be run in several modes:

1. With no compile database:

`clang-tidy` will attempt to guess compile options. This isn't ideal but will probably work in most cases.

`clang-tidy -checks "-*,eastwood*" path-to-file.c`

2. With a compile database:

`clang-tidy` will use compile options from a compilation database. This can be made by hand or output by cmake. 

To generate the database with cmake, call cmake with `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON`

To generate the database by hand, simply follow the below format and create `compile_commands.json` in the same directory (or a parent directory) of your `.c` files.

```
[
    {
        "directory": "/absolute/path/to/the/output/directory/of/cmake/or/other/build/system/",
        "command": "gcc -g -ggdb -O0 -any -other -compile -options -here -o relative/or/absolute/path/to/outputfile.o -c /absolute/path/to/the/c/file.c",
        "file": "/absolute/path/to/the/c/file.c"
    },
    {
        ...
    },
    ...
]

```

If that's unparseable, basically it's a dict containing the directory of the output (ie directory to build from), the compile command (gcc -whatever -o thing thing.c), and the file to compile.

The compile database can be manually specified with `clang-tidy -p <database>.json`, but it is probably easier to just put it in the same place as your source (or 1 directory above).

The command `clang-tidy -checks "-*,eastwood*" path-to-file.c` is the same.

## Testing <a name="testing" />

`eastwood-tidy` can be tested against the test cases in the `test` directory by building the tests and compilation database with a provided path to the built `clang-tidy` binary.

```
$ cd test
$ mkdir build
$ cd build
$ cmake -DCMAKE_EXPORT_COMPILE_COMMANS=ON -DCLANG_TIDY_LOCATION=/absolute/path/to/clang-tidy ..
$ make -j NN
```

The tests are demarcated in `test/test-spec.json` and run using python's `unittest` framework. I know, I know, this is a C project. Python is easier. Sue me.

## Notest <a name="notes" />

Note for include directories and files: using `clang-tidy <regular args> -- <clang args>` can be done to specify include directories. For example: `./clang-tidy -checks "-*,eastwood*" /home/novafacing/hub/llvm-project/clang-tools-extra/clang-tidy/eastwood/test/I/test_I_D_fail.c -- -I/nix/store/lqn6r231ifgs2z66vvaav5zmrywlllzf-glibc-2.31-dev/include/`
