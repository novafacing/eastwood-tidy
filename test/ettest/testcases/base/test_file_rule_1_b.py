"""
File tests for Rule1b:
"Variable names should be descriptive and meaningful."
"""

from ettest.testcases.snippets.snippets import Error
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest

def test_rule_1_b_file(manager) -> None:
    """
    File test cases for rule 1b
    """
    tests = [
        FileTest("eastwood-rule-1b.c", [])
    ]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors
