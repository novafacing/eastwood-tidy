# eastwood-tidy


## Table Of Contents
1. [About This Project](#about)
2. [Users](#users)
  * [Installation](#installation)
  * [Usage](#usage)
3. [Developers](#developers)
  * [Building](#building)
  * [Linting](#linting)
  * [Testing](#testing)
4. [Notes](#notes)
7. [Code Standard Reference](#code-standard-reference)

## About

`eastwood-tidy` is a port of the Eastwood C language linter to the clang-tidy check system.

Initial work was done by Connor McMillin. The project is now maintained by Rowan Hart
<rowanbhart@gmail.com>.

## Users

Users of eastwood-tidy do *not* need to follow the [developer](#developers) documentation
to download and install LLVM, and should instead prefer to obtain a binary distribution
of the linter.

### Installation

#### Dependencies

Note that you will need to have `llvm-dev` installed on your system if you want
to avoid spurious errors for missing header files. Otherwise, there are no other
dependencies and the distributed binary is statically linked.

#### Downloading and Installing

You can obtain the latest release of `eastwood-tidy` by downloading the latest binary
from the [releases](https://github.com/novafacing/eastwood-tidy/releases) page on
GitHub. `eastwood-tidy` currently only supports Linux, and there are no plans to create
releases for other platforms. You are, of course, welcome to create these yourself.


The binary can be installed by simply copying it to a `$PATH` location, or with a full
path. 
### Usage

#### Recommended Running

The recommended way to run `eastwood-tidy` is through the [linter](scripts/linter) script
to avoid needing to pass large numbers of arguments to the program. You may also choose
to write your own wrapper script, or you may modify the path given in ours and use it.

#### Manual Running

The program can be run in several modes:

1. With no compile database:

`clang-tidy` will attempt to guess compile options. This isn't ideal but will probably 
work in most cases.

`clang-tidy -checks "-*,eastwood*" path-to-file.c`

2. With a compile database:

`clang-tidy` will use compile options from a compilation database. This can be made by 
hand or output by cmake.

To generate the database with cmake, call cmake with `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON`

To generate the database by hand, simply follow the below format and create
`compile_commands.json` in the same directory (or a parent directory) of your `.c`
files.

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

If that's unparseable, basically it's a dict containing the directory of the output
(ie directory to build from), the compile command (gcc -whatever -o thing thing.c), and
the file to compile.

The compile database can be manually specified with `clang-tidy -p <database>.json`, but
it is probably easier to just put it in the same place as your source (or 1 directory
above).

The command `clang-tidy -checks "-*,eastwood*" path-to-file.c` is the same.

3. Options:

The clang-tidy linter has a few command line options, outlined below. Unfortunately,
they are taken in a rather nasty format. To pass options you can do:

```
$ clang-tidy -checks "-*,eastwood*" -config="{CheckOptions: [{key: a, value: b}, {key: x, value: y}]}" path-to-file.c
```

The options we provide (and an example usage) are below:

| Option                     | Default | Type | Example                                                                      | Description             |
| -------------------------- | ------- | ---- | ---------------------------------------------------------------------------- | ----------------------- |
| eastwood-Rule1bCheck.dump  | false   | bool | `-config="{CheckOptions: [{key: eastwood-Rule1bCheck.dump, value: true}]}"`  | Dump Names              |
| eastwood-Rule11dCheck.dump | false   | bool | `-config="{CheckOptions: [{key: eastwood-Rule11dCheck.dump, value: true}]}"` | Dump Embedded Constants |

## Developers


### Notes about developing

The recommended IDE for developing `eastwood-tidy` is VSCode. You can
develop it in VIM or Eclipse or anything you want, but rest assured it
will be a less user friendly experience.

Recommended plugin configuration for this project:

* Install `llvm-vs-code-extensions.vscode-clangd`
* Install `ms-vscode.cpptools`
* Use the repo `.vscode` settings which will *disable* `C_Cpp`
  intellisense and use `clangd` instead.

Below during the initial build there are instructions to add a
`compile_commands.json` symlink to provide `clangd`. Do this!
It will detect all the header information needed to view methods,
get autocompletion, and get error detection.

### Building

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

4. Run patches to remove extraneous un-toggleable error reports from clang-tidy about 
   missing compilation databases.

```bash
$ ./patch/patch.sh /path/to/llvm-project
```

5. Use CMake + Make to build the new clang-tidy

```
cd llvm-project/llvm
mkdir build
cd build
cmake -GNinja -DLLVM_TARGETS_TO_BUILD=X86 -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" -DCMAKE_CXX_COMPILER="clang++" -DCMAKE_C_COMPILER="clang" -DLLVM_USE_LINKER="lld" -DLLVM_BUILD_TESTS="OFF" -DCMAKE_BUILD_TYPE="Debug" -DBUILD_SHARED_LIBS="OFF" -DCMAKE_COMPILE_COMMANDS=1 ..
cmake --build . -j NN # where NN is the number of cores in your machine + 1
```

6. If you are using vscode, you will want to symlink the `compile_commands.json` into your development directory `ln -s $(pwd)/compile_commands.json /path/to/eastwood-tidy/compile_commands.json` to allow `clangd` to work.

7. Binary will be located at `llvm-project/llvm/build/bin/clang-tidy`

8. If developing for use at Purdue University, the binary can be updated
   by using the [update script](scripts/update.sh) which will upload both
   the new binary and the necessary include directories to avoid spurious errors.

### Linting

There is a `.clang-format` file provided in the root directory. Ensure that your
`clang-format` utility uses this format file.

To facilitate linting, a `pre-commit` configuration is provided. You can run
`pre-commit install` in the root of the repository to install the hook to format
the `src` and `include` directories on commit.

Note that improperly formatted code in Pull Requests will not be accepted, and you
will be gently asked to run the formatter and update your request. I recognize that
everyone has their preferred style and this may not be yours, but this is the source
code for a linter, after all! Let's keep it consistent.

### Testing

Tests have been updated once again to use a bespoke pytest-based testing framework.

#### Install Test Framework

To run tests, you will need to install the virtual environment for the tests:

```sh
cd test
poetry install
```

If you do not have `poetry`, you can get it
[here](https://python-poetry.org/blog/announcing-poetry-1.2.0a1/).

#### Running Tests

To run the tests, first make sure you are in the virtual environment. You can either run
`poetry shell` (recommended) to work on the environment in your shell, or you can
prepend `poetry run` to any test commands you run.

Specific instructions on writing and running tests in various forms can be found
in the [testing readme](test/README.md), but in general you can run all of the tests
by running `pytest`. There are currently a large number of failing and non-implemented
tests due to the in-progress overhaul of the codebase for the project.

## Notes

### Note for NixOS users:

To find your glibc directory, just `nix eval nixpkgs.glibc.dev.outPath | tr -d '\n' | tr -d '"' | cat <<< "/include/"`.

Where `glibc.dev` is any library whose headers you need.

You can just do `clang-tidy <args> -- $(nix eval nixpkgs.glibc.dev.outPath | tr -d '\n' | tr -d '"' | cat <<< "/include")`.


### Note for include directories

Note for include directories and files: using `clang-tidy <regular args> -- <clang args>` 
can be done to specify include directories. For example: 

```
./clang-tidy -checks "-*,eastwood*" /home/novafacing/hub/llvm-project/clang-tools-extra/clang-tidy/eastwood/test/I/test_I_D_fail.c -- -I/nix/store/lqn6r231ifgs2z66vvaav5zmrywlllzf-glibc-2.31-dev/include/
```

## Code Standard Reference

### I. NAMING CONVENTION

The following rules must be applied to all functions, structures,
typedefs, unions, variables, etc.

#### A. Variable names should be in all lowercase.

If the name is composed of more than one word, then underscores
must be used to separate them.

Example: int temperature = 0;

Example: int room_temperature = 0;

#### B. Use descriptive and meaningful names.

Example: Variable such as "room_temperature" is
descriptive and meaningful, but "i" is not.  An exception can
be made if "i" is used for loop counting, array indexing, etc.

An exception can also be made if the variable name is something
commonly used in a mathematical equation, and the code is
implementing that equation.

#### C. All constants must be all uppercase, and contain at least two characters.  

Constants must be declared using #define.
A constant numeric value assigned must be enclosed in
parenthesis.

String constants need to be placed in quotes but do not
have surrounding parentheses.

Example: `#define TEMPERATURE_OF_THE_ROOM (10)`

Example: `#define FILE_NAME  "Data_File"`

#### D. All global variables must be started with prefix "g_".

Declarations/definitions should be at the top of the file.

Example: `int g_temperature = 0;`

Global variable use should be avoided unless absolutely necessary.

### II. LINE AND FUNCTION LENGTH

A. Each line must be kept within 80 columns in order to make sure
the entire line will fit on printouts.  If the line is too long,
then it must be broken up into readable segments.
The indentation of the code on the following lines needs to be
at least 2 spaces.


Example: 
```c
room_temperature = list_head->left_node->
                  left_node->
                  left_node->
                  left_node->
                  left_node->
                  temperature;

```
Example: 

```c
fread(&value, sizeof(double),
1, special_fp);
```

B. Each function should be kept small for modularity purpose.
The suggested size is less than two pages.
Exception can be made, if the logic of the function requires its
size to be longer than two pages. Common sense needs to be followed.

Example: If a function contains more than two pages of printf
or switch statements, then it would be illogical to break
the function into smaller functions.

### III. SPACING

#### A. One space must be placed after all structure control, and flow commands. 

One space must also be present between the closing
parenthesis and opening brace.

Example: `if (temperature == room_temperature) {`

Example: `while (temperature < room_temperature) {`

#### B. One space must be placed before and after all logical, and arithmetic operators; 

Exception for unary and data reference
operators `(i.e. [], ., &, *, ->)`.

Example: `temperature = room_temperature + offset;`

Example: `temperature = node->data;`

Example: `if (-temperature == room_temperature)`

Example: `for (i = 0; i < limit; ++i)`

Example: `*value = head->data;`

#### C. One space must be placed after internal semi-colons and commas.

Example: `for (i = 0; i < limit; ++i)`

Example: `printf("%f %f %f\n", temperature, volume, area);`

#### D. #define expressions need to be grouped together and need to be lined up in column 1. 

They need to have a blank line
above and below.

Example: 

```c
int whatever(int some_value) {
  
  #define FUNCTION_NAME  "Whatever"
  #define UPPER_LIMIT (56)
  
  . . .
  
} /* whatever() */
```

#### E. Never put trailing whitespace at the end of a line.

#### F. Never place spaces between function names and the parenthesis preceding the argument list.

### IV. INDENTATION

#### A. Two space indentation must be applied to all structure, control, and flow commands.  

This two space indentation rule
must be applied to the entire program.

Note that the opening brace must be placed on the same line as the
structure, control, or flow command.  The closing brace must be
placed on the line after the structure, control, or flow commands.
The closing brace must also be alone on the line. Even if only one
statement is to be executed it is necessary to use braces.

Example: 
```c
for (i = 0; i <= size; ++i) {
  average += data[i];
}
```

Example: 
```c
while (temperature < MAX_TEMPERATURE) {
  average += data[i];
  temperature += offset;
}
```

Bad Example: 
```c
if (x < 7) {
  . . .
} else {  /* NO: else { should be on the next line */
  . . .
}
```

#### B. Parameters for functions with more than one parameter should be on the same line, unless the line length is exceeded. 

In that case,
parameters on the next line should begin at the same column position
as the parameters on the first line. The example below uses fewer
than 80 characters just for demonstration purposes.

Example: 
```c
double average(double *data, int size, char *name,
      int temperature) {
  . . .
} /* average() */
```

Example: 
```c
int main(void) {
  . . .
} /* main() */
```

#### C. Do while loops need to be indented with the while on the same line as the closing brace.

Example: 
```c
do {
  . . .
} while (size < LIMIT);
```

### V. COMMENTS

#### A. Comment should be meaningful and not repeat the obvious.

Comments are intended to alert people the intention of the code.

Example: This is a bad comment.

```c
/* Variable to store the temperature */

double temperature = 0.0;
```

Example: This is a bad comment.

```c
/* Increment i by one */

++i;
```

Example: This is a good comment.

```c
/* Temperature is measured in Celsius */
/* and it ranges from 0 - 150 degrees */

double temperature = 0.0;
```

#### B. Place comments above the code rather than along side the code.
Exceptions can be made for short comments placed beside declarations,
else, and switch commands.

Example: 
```c
if (temperature == room_temperature) {
  statement;
  statement;
}
else {     /* comment for else statement */
  statement;
  statement;
}
```

Example: 
```c
switch (key) {
  case DO_THIS: {       /* comment for DO_THIS */
    statement;
    statement;
    break;
  }
  case DO_THAT: {       /* comment for DO_THAT */
    statement;
    statement;
    break;
  }
  default: {            /* comment for defaults */
    statement;
    statement;
    break;
  }
}
```

#### C. Code and comments must be separated by blank lines, and lined up.
A blank line is not required above the comment if it is the 1st.
line following an opening brace.

Example: 
```c
if (temperature == room_temperature) {
  statement;
  
  /* Comment */
  
  statement;
}
else {
  /* Comment for action when temperature is */
  /* not equal room_temperature             */

  statement;
}
```

#### D. Function's name must be commented at the end of each function.
The comment should be the name of the function indented one
space after the closing brace and include left and right
parentheses.

Example: 

```c
double average(double *data, int size) {
  . . .
} /* average() */
```

#### E. Function header comments should have a blank line above and below the comment.

See section VII for an example.

### VI. MULTIPLE LOGICAL EXPRESSIONS

If multiple logical expressions are used, sub-expressions must be
parenthesized. Note the spacing and format below.

Example: 
```c
if ((volume_box_a == volume_box_b) &&
  (volume_box_b == volume_box_c)) {
  . . .
}
```

Example  

```c
if (((side_a < side_b) && (time < max_time)) ||
  ((value  < data) && (limit > MIN_VALUE))) {
  . . .
}
```

### VII. HEADERS

A header must be placed at the beginning of each function (including
the main program).  A header must contain detailed information, which
describes the purpose of the function.  The format is defined below:
The header comment block must be at the left edge.

Example: Header at left edge.

```c
/*
*  This function computes the average of the passed data,
*  which is stored in an array pointed to by ptr_data.
*  The parameter Size is the index of the last array
*  element used. The array uses items 0 to Size.
*/

double average(double *data, int size) {
  int i = 0;
  double average = 0.0;

  for (i = 0; i <= size; ++i) {
    average += data[i];
  }

  return (average / (size + 1));

} /* average() */
```

### VIII. HEADER FILES

#### A. In general, every .c file should have an associated .h file.

#### B. Header files should be self-contained (compile on their own) and end in .h.

#### C. All header files should have #define guards to prevent multiple inclusions. The format of the symbol name should be `_H`.

Example:

```c
#ifndef BAZ_H
#define BAZ_H

...

#endif // BAZ_H
```

#### D. All project header files should be listed as descendants of the project's source directory. 

UNIX directory shortcuts (e.g., . and ..) are forbidden.

Example: src/base/logging.h should be included as:

```c
#include "base/logging.h"
```

#### E. Includes should be ordered per the following example. 

Suppose dir/foo.c implements things in dir2/foo.h. Your includes should be ordered as follows:

```c
#include "dir2/foo.h"

#include <sys/types.h>
#include <unistd.h>

#include "base/basictypes.h"
#include "base/commandflags.h"
#include "foo/server/bar.h"
```

Note the spaces. Any adjacent blank lines should be collapsed.

#### F. All relevant include files should be explicitly included. 

Do not assume that just because foo.h currently includes bar.h you do not 
need to explicitly include bar.h.

#### G. Non-local includes (standard libraries, etc) must be surrounded by `<` and `>`. 

Double quotes should only be used for include files that exist in the local directory structure.

### IX. DEFENSIVE CODING TECHNIQUE

#### A. Return values of functions such as malloc, calloc, fopen, fread, fwrite, and system must be checked or returned whenever a possible error condition exists.

Example: 

```c
if ((data_fp = fopen(file_name, "r")) == NULL) {
  fprintf(stderr, "Error: Cannot open file ");
  fprintf(stderr, "%s.\n", file_name);
  exit(TRUE);
}
```

#### B. When the function fopen is invoked to open a file, fclose must be used to close the file.  

It is important to remember that
fclose does not explicitly set the file pointer back to NULL.
Therefore, it is necessary to set the file pointer to NULL.

#### C. After dynamically deallocating a pointer using free, the pointer must be set back to NULL.

#### D. Appropriate range checking must be performed to make sure received parameters are within expected range.

Example: 

```c
if ((temperature < LOWER_BOUND) ||
  (temperature > HIGHER_BOUND)) {
  fprintf(stderr, "Error: Temperature out of range.\n");
  exit(TRUE);
}
```

Example:  
```c
assert((temperature > LOWER_BOUND) &&
  (temperature <= HIGHER_BOUND));
```

#### E. The appropriately typed symbol should be used to represent 0 and NULL.

Integers: Use `0`
Reals: Use `0.0`
Pointers: Use `NULL`
Characters: Use `'\0'`

#### X. OUTPUT HANDLING

All error messages must be directed to standard error.

Example: `fprintf(stderr, "Error has occurred.\n");`


XI. FORBIDDEN STATEMENTS

#### A. Do not use tabs for indentation.

#### B. Use only UNIX newline encoding (\n). DOS newlines (\r\n) are prohibited.

#### C. Do not make more than one assignment per expression.

Bad Example: `volume_box_a = volume_box_b = volume_box_c;`

Bad Example: 
```c
if ((volume_box_a = compute_volume_box_a()) +
  (volume_box_b = compute_volume_box_b()))
```

#### D. Do not use embedded constants; except for general initialization purposes and values that lack intrinsic meaning.

Common sense needs to apply.

Bad Example: `return 1;                /* DO NOT USE **/`

In this case, the return value represents a failure.
Create a #defined constant at the beginning of your
program instead and use that...

```c
#define HAILSTONE_ERROR  (1)
```

Bad Example: `if (temperature > 10)   /* DO NOT USE */`
Again, in this case the value means something, and you
should define a constant instead...

```c
#define MAX_TEMPERATURE  (10)
```

Bad Example: `int temperature = 10;    /* DO NOT USE */`
Same idea...
```c
#define START_TEMP  (10)
```

Bad Examples: 

```c
#define ONE  (1)
#define FIRST_INDEX  (0)
#define FIRST_ITERATION  (0)
```

If the value has intrinsic meaning, that meaning should
be conveyed in the constant's name.

Example: 
```c
int temperature = 0;     /*  OK if 0 has no
                 *  specific meaning
                 */
```

Some exceptions:

Example: `somefile_fp = fopen(file_name, "rb");`

The use of the embedded constant "rb" is ok.

Example: `if (fread(&data, sizeof(int), 2, somefile_fp) == 2)`

The embedded constants 2 are ok here.

Example: `if (fscanf(fp, "%d %s %f", &my_int, my_str, &my_float) == 3)`

The embedded constant 3 is okay here.

Example: `some_string[MAX_SIZE - 1] = '\0';`

Subtracting or adding one from/to the size for the NULL
terminator is fine.

Example: `if (strchr(some_buf, 's')) {`

You do not need to define a constant when referring to
single, human-readable characters.

Example: `malloc(sizeof(something) * 4);  /* Allocate space for 4 somethings */`

The constant 4 here is okay.

Again, please use common sense.

#### E. The use of goto is forbidden in this course.

### XII. VARIABLE DECLARATIONS

#### A. No more than one variable may be defined on a single line.

DON'T DO THIS:
```c

int side_a, side_b, side_c = 0;
```

Do it this way:

```c
int side_a = 0;
int side_b = 0;
int side_c = 0;
```


#### B. All variables must be initialized at the time they are defined.

#### C. Variables should be placed in as local a scope as possible, as close to the first use as possible.

Example:
```c
while (const char *p = strchr(str, '/')) {
  str = p + 1;
}
```