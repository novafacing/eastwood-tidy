"""
File tests for Rule9a:
"Return values of library functions must be checked."
"""

from ettest.testcases.snippets.snippets import Error
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest

def test_rule_9_a_file(manager) -> None:
    """
    File test cases for rule 9a
    """
    tests = [
        FileTest("eastwood-rule-9a.c", [])
    ]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors
