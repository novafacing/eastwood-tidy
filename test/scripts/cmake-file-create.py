import json
import os
import sys
from collections import OrderedDict

CMAKE_FILE_TOP_TEMPLATE = """
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
"""

RULE_TEMPLATE = """
####################################
####  TEST {}.{} {}
####################################

add_executable(test_{}_{}_{}
    src/{}/{}
)

target_compile_options(test_{}_{}_{}
    PRIVATE
        -g
        -ggdb
        -O0
)
"""

def write_roman(num):
    roman = OrderedDict()
    roman[1000] = "M"
    roman[900] = "CM"
    roman[500] = "D"
    roman[400] = "CD"
    roman[100] = "C"
    roman[90] = "XC"
    roman[50] = "L"
    roman[40] = "XL"
    roman[10] = "X"
    roman[9] = "IX"
    roman[5] = "V"
    roman[4] = "IV"
    roman[1] = "I"

    def roman_num(num):
        for r in roman.keys():
            x, y = divmod(num, r)
            yield roman[r] * x
            num -= (r * x)
            if num <= 0:
                break

    return "".join([a for a in roman_num(num)])

def generate_rule(section, subsection, spec):
    rules = []
    rules.append(RULE_TEMPLATE.format(write_roman(int(section)), subsection.upper(), "PASS",
        write_roman(int(section)), subsection.upper(), "pass",
        write_roman(int(section)), "test_{}_{}_pass.c".format(write_roman(int(section)), subsection.upper()),
        write_roman(int(section)), subsection.upper(), "pass"))
    rules.append(RULE_TEMPLATE.format(write_roman(int(section)), subsection.upper(), "FAIL",
        write_roman(int(section)), subsection.upper(), "fail",
        write_roman(int(section)), "test_{}_{}_fail.c".format(write_roman(int(section)), subsection.upper()),
        write_roman(int(section)), subsection.upper(), "fail"))
    return rules

def process(spec):
    rules = []
    for section in spec:
        for subsection in spec[section]:
            if spec[section][subsection]["test"]:
                rules.extend(generate_rule(section, subsection, spec[section][subsection]))
    with open("CMakeLists.txt", "w") as cmakelists:
        cmakelists.write(CMAKE_FILE_TOP_TEMPLATE)
        cmakelists.write("\n".join(rules))

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: {} specfile.json".format(sys.argv[0]))
    elif len(sys.argv) == 2:
        process(json.load(open(sys.argv[1], "r")))
