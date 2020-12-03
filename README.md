# eastwood-tidy


## Table Of Contents
1. [About This Project](#about)
2. [Building](#building)
3. [Installation](#installation)
4. [Usage](#installation)
5. [Testing](#testing)
6. [Notes](#notes)
7. [Wiki](https://github.com/novafacing/eastwood-tidy/wiki)

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

3. Options:

The clang-tidy linter has a few command line options, outlined below. Unfortunately, they are taken in a rather nasty format. To pass options you can do:

```
$ clang-tidy -checks "-*,eastwood*" -config="{CheckOptions: [{key: a, value: b}, {key: x, value: y}]}" path-to-file.c
```

The options we provide (and an example usage) are below:

| Option                    | Default | Type | Example                                                                      | Description             |
| ------                    | ------- | ---- | -----------------------------------------------------------------------------| ------------------------|
| eastwood-Rule1bCheck.dump | false   | bool | `-config="{CheckOptions: [{key: eastwood-Rule1bCheck.dump, value: true}]}"`  | Dump Names              |
| eastwood-Rule1cCheck.dump | false   | bool | `-config="{CheckOptions: [{key: eastwood-Rule1cCheck.dump, value: true}]}"`  | Dump Embedded Constants |

## Testing <a name="testing" />

#### The new test procedure is to do the following:

1. Install the dependencies for LLVM however is appropriate for your system. You need at minimum the `FileCheck` utility.

2. Copy the script `scripts/check_clang_tidy.py` into your LLVM repo: `llvm-project/clang-tools-extra/test/clang-tidy/check_clang_tidy.py`.

3. Set the `CLANG_TIDY` variable at the top of the script to your `clang-tidy` location. Probably this is `llvm-project/llvm/build/bin/clang-tidy`.

4. `cp -av eastwood/test/*.c llvm-project/clang-tools-extra/test/clang-tidy/checkers/`

5. `cp -av eastwood/test/*.h llvm-project/clang-tools-extra/test/clang-tidy/checkers/`

The above 2 points just mean: move all the .c and .h test files into the checkers directory from wherever `eastwood` is lcoated.

6. Run the checker: `python3 check_clang_tidy.py checkers/eastwood-rule-1a.c eastwood-Rule1A ./checkers/`

Where eastwood-rule-1a.c and eastwood-Rule1A are the name of a source file and a code standard point, respectively.


#### The below is deprecated guidance, use at your own risk.

`eastwood-tidy` can be tested against the test cases in the `test` directory by building the tests and compilation database with a provided path to the built `clang-tidy` binary.

```
$ cd test
$ mkdir build
$ cd build
$ cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCLANG_TIDY_LOCATION=/absolute/path/to/clang-tidy ..
$ make -j NN
$ make test
```

Alternatively, you can just run `ctest` in the `build` directory.

The tests are demarcated in `test/test-spec.json` and run using python's `unittest` framework. I know, I know, this is a C project. Python is easier. Sue me.

*Note*: currently there is a variable `EXTRA_ARGS` in test.py. This is because I use NixOS. Just make that an empty array for your own testing...until I fix it :)

## Notes <a name="notes" />

### Note for NixOS users:

To find your glibc directory, just `nix eval nixpkgs.glibc.dev.outPath | tr -d '\n' | tr -d '"' | cat <<< "/include/"`.

Where `glibc.dev` is any library whose headers you need.

You can just do `clang-tidy <args> -- $(nix eval nixpkgs.glibc.dev.outPath | tr -d '\n' | tr -d '"' | cat <<< "/include")`.


### Note for include directories

Note for include directories and files: using `clang-tidy <regular args> -- <clang args>` can be done to specify include directories. For example: `./clang-tidy -checks "-*,eastwood*" /home/novafacing/hub/llvm-project/clang-tools-extra/clang-tidy/eastwood/test/I/test_I_D_fail.c -- -I/nix/store/lqn6r231ifgs2z66vvaav5zmrywlllzf-glibc-2.31-dev/include/`
