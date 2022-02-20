"""
File tests for Rule3c:
"One space after all internal commas and semicolons."
"""

from ettest.testcases.snippets.snippets import Error
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest

def test_rule_3_c_file(manager) -> None:
    """
    File test cases for rule 3c
    """
    tests = [
        FileTest("eastwood-rule-3c.c", [])
    ]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors
