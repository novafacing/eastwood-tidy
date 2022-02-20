"""
File tests for Rule9d:
"Range checking should be performed on return values."
"""

from ettest.testcases.snippets.snippets import Error
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest

def test_rule_9_d_file(manager) -> None:
    """
    File test cases for rule 9d
    """
    tests = [
        FileTest("eastwood-rule-9d.c", [])
    ]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors
