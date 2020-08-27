import unittest
import sys
import os
import sys
import json
import re
import subprocess
from collections import OrderedDict

EXTRA_ARGS = ['--', '-I/nix/store/lqn6r231ifgs2z66vvaav5zmrywlllzf-glibc-2.31-dev/include/']

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


def runTest(clang_tidy_path, cfile_path):
    command = [clang_tidy_path, '-checks', '"-*,eastwood*"', cfile_path, *EXTRA_ARGS]
    proc = subprocess.run(command, stdout = subprocess.PIPE)
    return proc.stdout

class TestEastwood(unittest.TestCase):
    longMessage = True

def make_test(description, clang_tidy_path, cfile_path, section, subsection, fail):
    def test(self):
        result = runTest(clang_tidy_path, cfile_path).decode('utf-8')
        reg = '\[Rule ([MCDXLIV]+)\.([A-H])\]'
        matches = re.findall(reg, result)
        sects = [e for i, e in enumerate(matches) if i % 2 == 0]
        subs = [e for i, e in enumerate(matches) if i % 2 != 0]
        othersects = filter(lambda s: s != section, sects)
        othersubs = filter(lambda s: s != subsection, subs)
        for osec, osub in zip(othersects, othersubs):
            print("Warning: Extraneous error found: {}.{}".format(osec, osub))
        sects = filter(lambda s: s == section, sects)
        subs = filter(lambda s: s == subsection, subs)
        for sec, sub in zip(sects, subs):
            if fail:
                self.assertNotEqual(section, sec)
                self.assertNotEqual(subsection, sub)
            else:
                self.assertEqual(section, sec)
                self.assertEqual(subsection, sub)
    return test


if __name__ == "__main__":
    SOURCE_DIR = sys.argv[1]
    CLANG_TIDY_LOCATION = sys.argv[2]
    JSON_SPEC_FILE = sys.argv[3]
    sys.argv = [sys.argv[0]]
    spec = json.load(open(JSON_SPEC_FILE, 'r'))
    for section in spec:
        for subsection in spec[section]:
            if spec[section][subsection]["test"]:
                # Jesus....reformat this at some point
                test_func_pass = make_test("Test condition {} on Rule {}.{}".format("Pass", write_roman(int(section)), 
                    subsection.upper()), CLANG_TIDY_LOCATION, os.path.join(SOURCE_DIR, write_roman(int(section)), 
                    "test_{}_{}_pass.c".format(write_roman(int(section)), subsection.upper())), 
                    write_roman(int(section)), subsection.upper(), False)
                test_func_fail = make_test("Test condition {} on Rule {}.{}".format("Fail", write_roman(int(section)), 
                    subsection.upper()), CLANG_TIDY_LOCATION, os.path.join(SOURCE_DIR, write_roman(int(section)), 
                    "test_{}_{}_fail.c".format(write_roman(int(section)), subsection.upper())), 
                    write_roman(int(section)), subsection.upper(), True)

                setattr(TestEastwood, 'test_{}_{}_{}'.format(section, subsection.upper(), 'pass'), test_func_pass)
                setattr(TestEastwood, 'test_{}_{}_{}'.format(section, subsection.upper(), 'fail'), test_func_fail)

    unittest.main()

