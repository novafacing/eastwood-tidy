"""
File tests for Rule3a:
"One space after all structure/flow keywords (if, else, while, do, switch). One space before all open braces."
"""

from ettest.testcases.snippets.snippets import Error
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest

def test_rule_3_a_file(manager) -> None:
    """
    File test cases for rule 3a
    """
    tests = [
        FileTest("eastwood-rule-3a.c", [])
    ]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors
