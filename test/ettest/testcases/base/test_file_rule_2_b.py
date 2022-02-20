"""
File tests for Rule2b:
"Functions should fit on 2 pages (240 lines)."
"""

from ettest.testcases.snippets.snippets import Error
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest

def test_rule_2_b_file(manager) -> None:
    """
    File test cases for rule 2b
    """
    tests = [
        FileTest("eastwood-rule-2b.c", [])
    ]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors
