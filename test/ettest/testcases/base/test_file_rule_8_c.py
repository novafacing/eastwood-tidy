"""
File tests for Rule8c:
"All header files should have define guards to prevent multiple inclusion."
"""

from ettest.testcases.snippets.snippets import Error
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest

def test_rule_8_c_file(manager) -> None:
    """
    File test cases for rule 8c
    """
    tests = [
        FileTest("eastwood-rule-8c.c", [])
    ]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors
