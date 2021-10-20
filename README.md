# eastwood-tidy


## Table Of Contents
1. [About This Project](#about)
2. [Building](#building)
3. [Installation](#installation)
4. [Usage](#installation)
5. [Testing](#testing)
6. [Notes](#notes)
7. [Wiki](https://github.com/novafacing/eastwood-tidy/wiki)
8. [Code Standard Support](#code-standard)
9. [Original Code Standard](#original-code-standard)

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

4. Run patches to remove extraneous un-toggleable error reports from clang-tidy about missing compilation databases.

```bash
$ ./patch/patch.sh /path/to/llvm-project
```

5. Use CMake + Make to build the new clang-tidy

```
cd llvm-project/llvm
mkdir build
cd build
cmake -GNinja -DLLVM_TARGETS_TO_BUILD=X86 -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" -DCMAKE_CXX_COMPILER="clang++" -DCMAKE_C_COMPILER="clang" -DLLVM_USE_LINKER="lld" -DLLVM_BUILD_TESTS="OFF" -DCMAKE_BUILD_TYPE="Debug" -DBUILD_SHARED_LIBS="OFF" ..
cmake --build . -j NN # where NN is the number of cores in your machine + 1
```

6. Binary will be located at `llvm-project/llvm/build/bin/clang-tidy`

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

## Original Code Standard <a href="original-code-standard">

### I. NAMING CONVENTION

The following rules must be applied to all functions, structures,
typedefs, unions, variables, etc.

A. Variable names should be in all lowercase.

If the name is composed of more than one word, then underscores
must be used to separate them.

Example: int temperature = 0;

Example: int room_temperature = 0;

B. Use descriptive and meaningful names.

Example: Variable such as "room_temperature" is
descriptive and meaningful, but "i" is not.  An exception can
be made if "i" is used for loop counting, array indexing, etc.

An exception can also be made if the variable name is something
commonly used in a mathematical equation, and the code is
implementing that equation.

C. All constants must be all uppercase, and contain at least two
characters.  Constants must be declared using #define.
A constant numeric value assigned must be enclosed in
parenthesis.

String constants need to be placed in quotes but do not
have surrounding parentheses.

Example: `#define TEMPERATURE_OF_THE_ROOM (10)`

Example: `#define FILE_NAME  "Data_File"`

D. All global variables must be started with prefix "g_".
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

III. SPACING

A. One space must be placed after all structure control, and flow
commands. One space must also be present between the closing
parenthesis and opening brace.

Example: `if (temperature == room_temperature) {`

Example: `while (temperature < room_temperature) {`

B. One space must be placed before and after all logical, and
arithmetic operators; except for unary and data reference
operators `(i.e. [], ., &, *, ->)`.

Example: `temperature = room_temperature + offset;`

Example: `temperature = node->data;`

Example: `if (-temperature == room_temperature)`

Example: `for (i = 0; i < limit; ++i)`

Example: `*value = head->data;`

C. One space must be placed after internal semi-colons and commas.

Example: `for (i = 0; i < limit; ++i)`

Example: `printf("%f %f %f\n", temperature, volume, area);`

D. #define expressions need to be grouped together and need
to be lined up in column 1. They need to have a blank line
above and below.

Example: 

```c
int whatever(int some_value) {
  
  #define FUNCTION_NAME  "Whatever"
  #define UPPER_LIMIT (56)
  
  . . .
  
} /* whatever() */
```

E. Never put trailing whitespace at the end of a line.

F. Never place spaces between function names and the parenthesis
preceding the argument list.

IV. INDENTATION

A. Two space indentation must be applied to all structure,
control, and flow commands.  This two space indentation rule
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

B. Parameters for functions with more than one parameter should be on
the same line, unless the line length is exceeded. In that case,
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

C. Do while loops need to be indented with the while on the
same line as the closing brace.

Example: 
```c
do {
  . . .
} while (size < LIMIT);
```

V. COMMENTS

A. Comment should be meaningful and not repeat the obvious.
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

B. Place comments above the code rather than along side the code.
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

C. Code and comments must be separated by blank lines, and lined up.
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

D. Function's name must be commented at the end of each function.
The comment should be the name of the function indented one
space after the closing brace and include left and right
parentheses.

Example: 

```c
double average(double *data, int size) {
  . . .
} /* average() */
```

E. Function header comments should have a blank line above and
below the comment.

See section VII for an example.

VI. MULTIPLE LOGICAL EXPRESSIONS

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

VII. HEADERS

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

VIII. HEADER FILES

A. In general, every .c file should have an associated .h file.

B. Header files should be self-contained (compile on their own) and
end in .h.

C. All header files should have #define guards to prevent multiple
inclusions. The format of the symbol name should be _H.

Example:

```
#ifndef BAZ_H
#define BAZ_H

...

#endif // BAZ_H
```

D. All project header files should be listed as descendants of the
project's source directory. UNIX directory shortcuts
(e.g., . and ..) are forbidden.

Example: src/base/logging.h should be included as:

```
#include "base/logging.h"
```

E. Includes should be ordered per the following example. Suppose
dir/foo.c implements things in dir2/foo.h. Your includes should be
ordered as follows:

```
#include "dir2/foo.h"

#include <sys/types.h>
#include <unistd.h>

#include "base/basictypes.h"
#include "base/commandflags.h"
#include "foo/server/bar.h"
```

Note the spaces. Any adjacent blank lines should be collapsed.

F. All relevant include files should be explicitly included. Do not
assume that just because foo.h currently includes bar.h you do not
need to explicitly include bar.h.

G. Non-local includes (standard libraries, etc) must be surrounded by
< and >. Double quotes should only be used for include files that
exist in the local directory structure.

IX. DEFENSIVE CODING TECHNIQUE

A. Return values of functions such as malloc, calloc, fopen, fread,
fwrite, and system must be checked or returned whenever a possible
error condition exists.

Example: 

```
if ((data_fp = fopen(file_name, "r")) == NULL) {
  fprintf(stderr, "Error: Cannot open file ");
  fprintf(stderr, "%s.\n", file_name);
  exit(TRUE);
}
```

B. When the function fopen is invoked to open a file, fclose must
be used to close the file.  It is important to remember that
fclose does not explicitly set the file pointer back to NULL.
Therefore, it is necessary to set the file pointer to NULL.

C. After dynamically deallocating a pointer using free, the pointer
must be set back to NULL.

D. Appropriate range checking must be performed to make sure
received parameters are within expected range.

Example: 
```
if ((temperature < LOWER_BOUND) ||
  (temperature > HIGHER_BOUND)) {
  fprintf(stderr, "Error: Temperature out of range.\n");
  exit(TRUE);
}
```

Example:  
```
assert((temperature > LOWER_BOUND) &&
  (temperature <= HIGHER_BOUND));
```

E. The appropriately typed symbol should be used to represent 0 and
NULL.

Integers: Use `0`
Reals: Use `0.0`
Pointers: Use `NULL`
Characters: Use `'\0'`

X. OUTPUT HANDLING

All error messages must be directed to standard error.

Example: `fprintf(stderr, "Error has occurred.\n");`


XI. FORBIDDEN STATEMENTS

A. Do not use tabs for indentation.

B. Use only UNIX newline encoding (\n). DOS newlines (\r\n) are prohibited.

C. Do not make more than one assignment per expression.

Bad Example: `volume_box_a = volume_box_b = volume_box_c;`

Bad Example: 
```
if ((volume_box_a = compute_volume_box_a()) +
  (volume_box_b = compute_volume_box_b()))
```

D. Do not use embedded constants; except for general initialization
purposes and values that lack intrinsic meaning.
Common sense need to apply.

Bad Example: `return 1;                /* DO NOT USE **/`

In this case, the return value represents a failure.
Create a #defined constant at the beginning of your
program instead and use that...

```
#define HAILSTONE_ERROR  (1)
```

Bad Example: `if (temperature > 10)   /* DO NOT USE */`
Again, in this case the value means something, and you
should define a constant instead...

```
#define MAX_TEMPERATURE  (10)
```

Bad Example: `int temperature = 10;    /* DO NOT USE */`
Same idea...
```
#define START_TEMP  (10)
```

Bad Examples: 

```
#define ONE  (1)
#define FIRST_INDEX  (0)
#define FIRST_ITERATION  (0)
```

If the value has intrinsic meaning, that meaning should
be conveyed in the constant's name.

Example: 
```
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

E. The use of goto is forbidden in this course.

### XII. VARIABLE DECLARATIONS

A. No more than one variable may be defined on a single line.

DON'T DO THIS:
```

int side_a, side_b, side_c = 0;
```

Do it this way:

```
int side_a = 0;
int side_b = 0;
int side_c = 0;
```


B. All variables must be initialized at the time they are
defined.

C. Variables should be placed in as local a scope as possible, as
close to the first use as possible.

Example:
```
while (const char *p = strchr(str, '/')) {
  str = p + 1;
}
```