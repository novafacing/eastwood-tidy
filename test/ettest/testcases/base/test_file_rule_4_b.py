"""
File tests for Rule4b:
"Parameters should be on one line unless 80 columns is exceeded, in which case they should be aligned."
"""

from ettest.testcases.snippets.snippets import Error
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest

def test_rule_4_b_file(manager) -> None:
    """
    File test cases for rule 4b
    """
    tests = [
        FileTest("eastwood-rule-4b.c", [])
    ]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors
