"""
File tests for Rule12a:
"Do not define more than one variable on a single line."
"""

from ettest.testcases.snippets.snippets import Error
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest

def test_rule_12_a_file(manager) -> None:
    """
    File test cases for rule 12a
    """
    tests = [
        FileTest("eastwood-rule-12a.c", [])
    ]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors
