
cmake_minimum_required(VERSION 3.10)

macro(use_gcc)
    set(CMAKE_C_COMPILER gcc)
    set(CMAKE_CXX_COMPILER g++)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED True)
    set(C_STANDARD 11)
    set(C_STANDARD_REQUIRED True)
endmacro()

macro(use_clang)
    set(CMAKE_C_COMPILER clang)
    set(CMAKE_CXX_COMPILER clang)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED True)
    set(C_STANDARD 11)
    set(C_STANDARD_REQUIRED True)
endmacro()

enable_testing()
use_gcc()

project(eastwood-test VERSION 1.0)

####################################
####  TEST I.A PASS
####################################

add_executable(test_I_A_pass
    src/I/test_I_A_pass.c
)

target_compile_options(test_I_A_pass
    PRIVATE
        -g
        -ggdb
        -O0
)


####################################
####  TEST I.A FAIL
####################################

add_executable(test_I_A_fail
    src/I/test_I_A_fail.c
)

target_compile_options(test_I_A_fail
    PRIVATE
        -g
        -ggdb
        -O0
)


####################################
####  TEST I.C PASS
####################################

add_executable(test_I_C_pass
    src/I/test_I_C_pass.c
)

target_compile_options(test_I_C_pass
    PRIVATE
        -g
        -ggdb
        -O0
)


####################################
####  TEST I.C FAIL
####################################

add_executable(test_I_C_fail
    src/I/test_I_C_fail.c
)

target_compile_options(test_I_C_fail
    PRIVATE
        -g
        -ggdb
        -O0
)


####################################
####  TEST I.D PASS
####################################

add_executable(test_I_D_pass
    src/I/test_I_D_pass.c
)

target_compile_options(test_I_D_pass
    PRIVATE
        -g
        -ggdb
        -O0
)


####################################
####  TEST I.D FAIL
####################################

add_executable(test_I_D_fail
    src/I/test_I_D_fail.c
)

target_compile_options(test_I_D_fail
    PRIVATE
        -g
        -ggdb
        -O0
)


####################################
####  TEST IV.C PASS
####################################

add_executable(test_IV_C_pass
    src/IV/test_IV_C_pass.c
)

target_compile_options(test_IV_C_pass
    PRIVATE
        -g
        -ggdb
        -O0
)


####################################
####  TEST IV.C FAIL
####################################

add_executable(test_IV_C_fail
    src/IV/test_IV_C_fail.c
)

target_compile_options(test_IV_C_fail
    PRIVATE
        -g
        -ggdb
        -O0
)


####################################
####  TEST XI.E PASS
####################################

add_executable(test_XI_E_pass
    src/XI/test_XI_E_pass.c
)

target_compile_options(test_XI_E_pass
    PRIVATE
        -g
        -ggdb
        -O0
)


####################################
####  TEST XI.E FAIL
####################################

add_executable(test_XI_E_fail
    src/XI/test_XI_E_fail.c
)

target_compile_options(test_XI_E_fail
    PRIVATE
        -g
        -ggdb
        -O0
)
