"""
File tests for Rule6a:
"If multiple logical expressions are used, sub-expressions must be parenthesized."
"""

from ettest.testcases.snippets.snippets import Error
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest

def test_rule_6_a_file(manager) -> None:
    """
    File test cases for rule 6a
    """
    tests = [
        FileTest("eastwood-rule-6a.c", [])
    ]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors
