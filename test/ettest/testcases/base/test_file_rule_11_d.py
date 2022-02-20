"""
File tests for Rule11d:
"Do not use embedded constants."
"""

from ettest.testcases.snippets.snippets import Error
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest

def test_rule_11_d_file(manager) -> None:
    """
    File test cases for rule 11d
    """
    tests = [
        FileTest("eastwood-rule-11d.c", [])
    ]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors
