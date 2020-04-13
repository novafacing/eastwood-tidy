# eastwood-tidy
Port of Eastwood to Clang-Tidy Checks

Setup

1. Get the llvm github repo
    - git clone https://github.com/llvm/llvm-project.git
    
2. Put eastwood-tidy in llvm-project/clang-tools-extra/clang-tidy/eastwood
    
Make sure that the eastwood-tidy repo is named eastwood and is in llvm-project/clang-tools-extra/clang-tidy/ since the rest of the setup depends on this structure and naming

3. Copy files from eastwood/setup to the proper places in llvm-project/clang-tools-extra/clang-tidy/
    - cp eastwood/setup/CMakeLists.txt llvm-project/clang-tools-extra/clang-tidy/
    - cp eastwood/setup/ClangTidyForceLinker.h llvm-project/clang-tools-extra/clang-tidy/
    
4. Use CMake + Make to build the new clang-tidy
    - cd /llvm-project/llvm/
    - mkdir build
    - cd build
    - cmake -DLLVM_ENABLE_PROJECTS='clang;clang-tools-extra' ..
    - make -j NN clang-tidy
    
And now you should have the new clang-tidy with all the currently developed eastwood-tidy checks in it in llvm-project/llvm/build/bin.

To run the clang-tidy linter with only our rules you will need a few things:
  - test.c : a C file to check
  - build_commands.json : A json file of how to compile every C file. Put it in current directory and clang-tidy will find it

Command to run eastwood checks:
  clang-tidy -checks "-\*,eastwood\*" test.c
  
Progress
| Rule Number | Rule Sub-Letter | Rule Description | Implemented | Partially Implemented (Add description) | Will Not Implement |
|-------------|-------------|---------|-------------|-------------|------------|
| 1 | A | Variable names should be in all lowercase. If the name is composed of more than one word, then underscores    must be used to separate them |  |  |  |
|  | B | Use descriptive and meaningful names |  |  |  |
|  | C | All constants must be all uppercase, and contain at least two characters. Constants must be declared using #define. A constant numeric value assigned must be enclosed in parenthesis. |  |  |  |
|  | D | All global variables must be started with prefix "g_". Declarations/definitions should be at the top of the file. |  |  |  |
| 2 | A | Each line must be kept within 80 columns in order to make sure the entire line will fit on printouts.  If the line is too long, then it must be broken up into readable segments. The indentation of the code on the following lines needs to be at least 2 spaces. |  |  |  |
|  | B | Each function should be kept small for modularity purpose. The suggested size is less than two pages. Exception can be made, if the logic of the function requires its size to be longer than two pages. Common sense needs to be followed. |  |  |  |
| 3 | A | One space must be placed after all structure control, and flow commands. One space must also be present between the closing parenthesis and opening brace. |  |  |  |
|  | B | One space must be placed before and after all logical, and arithmetic operators; except for unary and data reference operators (i.e. [], ., &, \*, ->). |  |  |  |
|  | C | One space must be placed after internal semi-colons and commas. |  |  |  |
|  | D | #define expressions need to be grouped together and need to be lined up in column 1. They need to have a blank line above and below. |  |  |  |
|  | E | Never put trailing whitespace at the end of a line. |  |  |  |
|  | F | Never place spaces between function names and the parenthesis preceding the argument list. |  |  |  |
| 4 | A | Two space indentation must be applied to all structure, control, and flow commands. This two space indentation rule must be applied to the entire program. |  |  |  |
|  | B | Parameters for functions with more than one parameter should be on the same line, unless the line length is exceeded. In that case, parameters on the next line should begin at the same column position as the parameters on the first line. The example below uses fewer than 80 characters just for demonstration purposes. |  |  |  |
|  | C | Do while loops need to be indented with the while on the same line as the closing brace. |  |  |  |
| 5 | A | Comment should be meaningful and not repeat the obvious. Comments are intended to alert people the intention of the code.   |  |  |  |
|  | B | Place comments above the code rather than along side the code. Exceptions can be made for short comments placed beside declarations, else, and switch commands. |  |  |  |
|  | C | Code and comments must be separated by blank lines, and lined up. A blank line is not required above the comment if it is the 1st. line following an opening brace. |  |  |  |
|  | D | Function's name must be commented at the end of each function. The comment should be the name of the function indented one space after the closing brace and include left and right parentheses. |  |  |  |
|  | E | Function header comments should have a blank line above and below the comment. |  |  |  |
| 6 |  | If multiple logical expressions are used, sub-expressions must be parenthesized. |  |  |  |
| 7 |  | A header must be placed at the beginning of each function (including the main program). A header must contain detailed information, which describes the purpose of the function. The header comment block must be at the left edge. |  |  |  |
| 8 | A | In general, every .c file should have an associated .h file. |  |  |  |
|  | B | Header files should be self-contained (compile on their own) and end in .h. |  |  |  |
|  | C | All header files should have #define guards to prevent multiple inclusions. The format of the symbol name should be \_H. |  |  |  |
|  | D | All project header files should be listed as descendants of the project's source directory. UNIX directory shortcuts  |  |  |  |
|  | E | Includes should be ordered per the following example. Suppose dir/foo.c implements things in dir2/foo.h. Your includes should be ordered as follows: |  |  |  |
|  | F | All relevant include files should be explicitly included. Do not assume that just because foo.h currently includes bar.h you do not need to explicitly include bar.h.
 |  |  |  |
|  | G | Non-local includes (standard libraries, etc) must be surrounded by < and >. Double quotes should only be used for include files that exist in the local directory structure. |  |  |  |
| 9 | A | Return values of functions such as malloc, calloc, fopen, fread, fwrite, and system must be checked or returned whenever a possible error condition exists. |  |  |  |
|  | B | When the function fopen is invoked to open a file, fclose must be used to close the file.  It is important to remember that fclose does not explicitly set the file pointer back to NULL. Therefore, it is necessary to set the file pointer to NULL. |  |  |  |
|  | C | After dynamically deallocating a pointer using free, the pointer must be set back to NULL. |  |  |  |
|  | D | Appropriate range checking must be performed to make sure received parameters are within expected range. |  |  |  |
|  | E | The appropriately typed symbol should be used to represent 0 and NULL. |  |  |  |
| 10 |  | All error messages must be directed to standard error. |  |  |  |
| 11 | A | Do not use tabs for indentation. |  |  |  |
|  | B | Use only UNIX newline encoding (\n). DOS newlines (\r\n) are prohibited. |  |  |  |
|  | C | Do not make more than one assignment per expression. |  |  |  |
|  | D | Do not use embedded constants; except for general initialization purposes and values that lack intrinsic meaning. Common sense need to apply. |  |  |  |
|  | E | The use of goto is forbidden in this course. |  |  |  |
| 12 | A | No more than one variable may be defined on a single line. |  |  |  |
|  | B | All variables must be initialized at the time they are defined. |  |  |  |
|  | C | Variables should be placed in as local a scope as possible, as close to the first use as possible. |  |  |  |
