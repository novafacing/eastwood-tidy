"""
File tests for Rule8b:
"Header files should end in .h."
"""

from ettest.testcases.snippets.snippets import Error
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest

def test_rule_8_b_file(manager) -> None:
    """
    File test cases for rule 8b
    """
    tests = [
        FileTest("eastwood-rule-8b.c", [])
    ]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors
