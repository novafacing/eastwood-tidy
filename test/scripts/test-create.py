import json
import os
import sys
from collections import OrderedDict

COMMENT_TEMPLATE = """/* --------------------- Test Code Style Section {}.{} {} """

COMMENT_LINE_TEMPLATE = """
 *
"""


MAIN_TEMPLATE = """
int test(void) {

}

int main(int argc, char ** argv) {
  printf("{0}\\n");
  test();
}
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

def format_template_end(formatted_template_start):
    template = ' * '
    for i in range(len(formatted_template_start) - len(' */   ')):
        template += '-'
    template += ' */'
    return template

def create_contents(if_pass, section, subsection, spec):
    comment = [COMMENT_TEMPLATE.format(write_roman(int(section)), subsection.upper(), "Pass" if if_pass else "Fail")]
    comment[0] += '-' * (80 - len(comment[0]))
    inner = [' * ' + spec["description"][i: i + 80 - len(' * ')] for i in range(0, len(spec["description"]), 80 - len(' * '))]
    comment.extend(inner)
    comment.append(format_template_end(comment[0]))
    comment.append('#include "test_{}_{}_{}.h"'.format(write_roman(int(section)), subsection.upper(), "pass" if if_pass else "fail"))
    return "\n".join(comment) + "\n" + MAIN_TEMPLATE

def format_new_test(if_pass, section, subsection, spec):
    print(section, subsection)
    print(spec)
    if spec["test"]:
        test = {}
        test["filename"] = 'test_{0}_{1}_{2}'.format(write_roman(int(section)), subsection.upper(), "pass" if if_pass else "fail")
        comment_contents = create_contents(if_pass, section, subsection, spec)
        test["test"] = comment_contents
        return test
    else:
        return None

def create_header_contents(section, subsection, filename, spec):
    include_guards = """#ifndef {0}_H
#define {1}_H
#include <stdio.h>

#endif // {2}_H""".format(filename.upper(), filename.upper(), filename.upper())
    return include_guards

def process(override, spec):
    for section in spec:
        for subsection in spec[section]:
            test = format_new_test(True, section, subsection, spec[section][subsection])
            if test is not None:
                if not os.path.isdir(os.path.join("src", write_roman(int(section)))):
                    os.mkdir(os.path.join("src", write_roman(int(section))))
                if override or not os.path.exists(os.path.join("src", write_roman(int(section)), test["filename"] + ".c")):
                    testfile = open(os.path.join("src", write_roman(int(section)), test["filename"] + ".c"), "w")
                    testfile.write(test["test"])
                if spec[section][subsection]["header"] and not os.path.exists(os.path.join("src", write_roman(int(section)), test["filename"] + ".h")):
                    header_contents = create_header_contents(section, subsection, test["filename"], spec[section][subsection])
                    headerfile = open(os.path.join("src", write_roman(int(section)), test["filename"] + ".h"), "w")
                    if override or not os.path.exists(test["filename"] + ".h"):
                        headerfile.write(header_contents)
            testf = format_new_test(False, section, subsection, spec[section][subsection])
            if testf is not None:
                if not os.path.isdir(os.path.join("src", write_roman(int(section)))):
                    os.mkdir(os.path.join("src", write_roman(int(section))))
                if override or not os.path.exists(os.path.join("src", write_roman(int(section)), testf["filename"] + ".c")):
                    testfile = open(os.path.join("src", write_roman(int(section)), testf["filename"] + ".c"), "w")
                    testfile.write(testf["test"])
                if spec[section][subsection]["header"] and not os.path.exists(os.path.join("src", write_roman(int(section)), testf["filename"] + ".h")):
                    header_contents = create_header_contents(section, subsection, testf["filename"], spec[section][subsection])
                    headerfile = open(os.path.join("src", write_roman(int(section)), testf["filename"] + ".h"), "w")
                    if override or not os.path.exists(testf["filename"] + ".h"):
                        headerfile.write(header_contents)

if __name__ == "__main__":
    if len(sys.argv) == 1:
        print("Usage: {} [-o] specfile.json".format(sys.argv[0]))
    if len(sys.argv) == 3 and sys.argv[1] == '-o':
        process(True, json.load(open(sys.argv[2], "r")))
    elif len(sys.argv) == 2:
        process(False, json.load(open(sys.argv[1], "r")))
