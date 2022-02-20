"""
File tests for Rule10a:
"All errors should be directed to stderr."
"""

from ettest.testcases.snippets.snippets import Error
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest

def test_rule_10_a_file(manager) -> None:
    """
    File test cases for rule 10a
    """
    tests = [
        FileTest("eastwood-rule-10a.c", [])
    ]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors
