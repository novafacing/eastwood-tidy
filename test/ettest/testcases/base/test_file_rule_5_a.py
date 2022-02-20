"""
File tests for Rule5a:
"Comments should be meaningful and not repeat the obvious."
"""

from ettest.testcases.snippets.snippets import Error
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest

def test_rule_5_a_file(manager) -> None:
    """
    File test cases for rule 5a
    """
    tests = [
        FileTest("eastwood-rule-5a.c", [])
    ]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors
