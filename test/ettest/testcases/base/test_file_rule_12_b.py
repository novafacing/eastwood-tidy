"""
File tests for Rule12b:
"All variables must be initialized when they are defined."
"""

from ettest.testcases.snippets.snippets import Error
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest

def test_rule_12_b_file(manager) -> None:
    """
    File test cases for rule 12b
    """
    tests = [
        FileTest("eastwood-rule-12b.c", [])
    ]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors
