"""
File tests for Rule8a:
"Every .c file should have an associated .h file."
"""

from ettest.testcases.snippets.snippets import Error
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest

def test_rule_8_a_file(manager) -> None:
    """
    File test cases for rule 8a
    """
    tests = [
        FileTest("eastwood-rule-8a.c", [])
    ]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors
