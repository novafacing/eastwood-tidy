"""
File tests for Rule1d:
"Global variables must begin with the prefix 'g_' and be located at the top of the file."
"""

from ettest.testcases.snippets.snippets import Error
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest

def test_rule_1_d_file(manager) -> None:
    """
    File test cases for rule 1d
    """
    tests = [
        FileTest("eastwood-rule-1d.c", [])
    ]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors
