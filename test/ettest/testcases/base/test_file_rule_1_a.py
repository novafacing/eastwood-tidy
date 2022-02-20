"""
File tests for Rule1a:
"Variable names should be all in lowercase, optionally separated by underscores."
"""

from ettest.testcases.snippets.snippets import Error
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest

def test_rule_1_a_file(manager) -> None:
    """
    File test cases for rule 1a
    """
    tests = [
        FileTest("eastwood-rule-1a.c", [])
    ]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors
