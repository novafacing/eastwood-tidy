"""
File tests for Rule5c:
"Comments must be preceded by either a blank line or an open brace."
"""

from ettest.testcases.snippets.snippets import Error
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest

def test_rule_5_c_file(manager) -> None:
    """
    File test cases for rule 5c
    """
    tests = [
        FileTest("eastwood-rule-5c.c", [])
    ]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors
