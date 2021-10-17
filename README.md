# eastwood-tidy


## Table Of Contents
1. [About This Project](#about)
2. [Building](#building)
3. [Installation](#installation)
4. [Usage](#installation)
5. [Testing](#testing)
6. [Notes](#notes)
7. [Wiki](https://github.com/novafacing/eastwood-tidy/wiki)
8. [Code Standard](#code-standard)

## About <a name="about" />

`eastwood-tidy` is a port of the Eastwood C language linter to the clang-tidy check system.

Initial work was done by Connor McMillin. The project is now maintained by Rowan Hart <rowanbhart@gmail.com>.

## Building <a name="building" />

1. Get the llvm github repo:

```bash
$ git clone https://github.com/llvm/llvm-project.git
```
    
2. Clone this repo:

```bash
$ git clone https://github.com/novafacing/eastwood-tidy.git
```

3. Link files from eastwood/setup to the proper places in llvm-project/clang-tools-extra/clang-tidy/

```bash
$ rm llvm-project/clang-tools-extra/clang-tidy/CMakeLists.txt llvm-project/clang-tools-extra/clang-tidy/ClangTidyForceLinker.h
$ ln -s $(pwd)/eastwood-tidy/setup/CMakeLists.txt $(pwd)/llvm-project/clang-tools-extra/clang-tidy/CMakeLists.txt
$ ln -s $(pwd)/eastwood-tidy/setup/ClangTidyForceLinker.h $(pwd)/llvm-project/clang-tools-extra/clang-tidy/ClangTidyForceLinker.h
$ ln -s $(pwd)/eastwood-tidy/ $(pwd)/llvm-project/clang-tools-extra/clang-tidy/
```
4. Use CMake + Make to build the new clang-tidy

```
cd llvm-project/llvm
mkdir build
cd build
cmake -GNinja -DLLVM_TARGETS_TO_BUILD=X86 -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" -DCMAKE_CXX_COMPILER="clang++" -DCMAKE_C_COMPILER="clang" -DLLVM_USE_LINKER="lld" -DLLVM_BUILD_TESTS="OFF" -DCMAKE_BUILD_TYPE="Debug" -DBUILD_SHARED_LIBS="OFF" ..
cmake --build . -j NN # where NN is the number of cores in your machine + 1
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

| Option                     | Default | Type | Example                                                                       | Description             |
| ------                     | ------- | ---- | ----------------------------------------------------------------------------- | ------------------------|
| eastwood-Rule1bCheck.dump  | false   | bool | `-config="{CheckOptions: [{key: eastwood-Rule1bCheck.dump, value: true}]}"`   | Dump Names              |
| eastwood-Rule11dCheck.dump | false   | bool | `-config="{CheckOptions: [{key: eastwood-Rule11dCheck.dump, value: true}]}"`  | Dump Embedded Constants |

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

## Code Standard <a name="code-standard" />

Below is the list of supported checks for the Eastwood-Tidy Linter. Checkboxes are only checked for full implementations.

### 1. Naming Convention

- [x] A. Variable names should be all in lowercase, optionally separated by underscores.
- [x] B. Variable names should be descriptive and meaningful.
- [x] C. Constants must be uppercase, contain >= 2 characters, be declared using ###define, have () for numbers and "" for strings.
- [x] D. Global variables must begin with the prefix "g_" and be located at the top of the file.

### 2. Line and Function Length

- [x] A. Lines must be under 80 columns. If it is too long, following lines must be indented at least 2 spaces.
- [x] B. Functions should fit on 2 pages (240 lines).

### 3. Spacing

- [x] One space after all structure/flow keywords (if, else, while, do, switch). One space before all open braces.
- [x] One space before and after all logical and arithmetic operators (+, -, *, /, ==, >, etc).
- [x] One space after all internal commas and semicolons.
- [ ] Define expressions should be grouped, and have a blank line above and below.

### 4. Indentation

- [x] Two space indentation must be applied to the entire program. Opening brace must be on the same line as keyword. Else must be on its own line.
- [x] Parameters should be on one line unless 80 columns is exceeded, in which case they should be aligned.
- [x] The while statement of a do-while loop must be on the same line as the closing brace.

### 5. Comments

- [ ] Comments should be meaningful and not repeat the obvious.
- [x] Comments must be placed above code except to annotate an if, else, or case statement.
- [x] Comments must be preceded by either a blank line or an open brace.
- [x] Function names must be commented at the end of each function in the format /* foo() */.
- [x] Function header comments should have a blank line preceding and following.

### 6. Multiple Logical expressions

- [x] If multiple logical expressions are used, sub-expressions must be parenthesized.

### 7. Headers

- [x] Header comments must be present for all functions, aligned to left edge.

### 8. Header Files.

- [x] Every .c file should have an associated .h file.
- [x] Header files should end in .h.
- [x] All header files should have define guards to prevent multiple inclusion.
- [x] All local header files should be descendants of the project; no `.`/`..` permitted.
- [x] Includes should be ordered by corresponding header, global includes, then local includes with each section in lexicographic order.
- [ ] All relevant files should be explicitly included.
- [x] Only non-local includes should used <...>, and local includes must use "...".

### 9. Defensive Coding

- [ ] Return values of library functions must be checked.
- [ ] File pointers from `fopen` must be closed with `fclose`.
- [ ] Pointers deallocated with `free` must be set to `NULL`.
- [ ] Range checking should be performed on return values.
- [ ] The appropriate zero should be used for `NULL` (0, 0.0, NULL, '\0').

### 10. Output Handling

- [ ] All errors should be directed to stderr.

### 11. Forbidden Statements

- [x] Never use tabs.
- [x] DOS newlines are prohibited.
- [x] Do not make multiple assignments in a single expression.
- [x] Do not use embedded constants.
- [x] Do not use goto.

### 12. Variable declarations

- [x] Do not define more than one variable on a single line.
- [x] All variables must be initialized when they are defined.
- [ ] Variables should be placed as locally as possible.