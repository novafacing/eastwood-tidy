"""
File tests for Rule3b:
"One space before and after all logical and arithmetic operators (+, -, *, /, ==, >, etc)."
"""

from ettest.testcases.snippets.snippets import Error
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest

def test_rule_3_b_file(manager) -> None:
    """
    File test cases for rule 3b
    """
    tests = [
        FileTest("eastwood-rule-3b.c", [])
    ]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors
