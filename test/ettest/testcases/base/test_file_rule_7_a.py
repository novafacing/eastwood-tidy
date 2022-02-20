"""
File tests for Rule7a:
"Header comments must be present for all functions, aligned to left edge."
"""

from ettest.testcases.snippets.snippets import Error
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest

def test_rule_7_a_file(manager) -> None:
    """
    File test cases for rule 7a
    """
    tests = [
        FileTest("eastwood-rule-7a.c", [])
    ]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors
