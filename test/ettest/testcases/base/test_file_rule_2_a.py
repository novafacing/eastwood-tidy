"""
File tests for Rule2a:
"Lines must be under 80 columns. If it is too long, following lines must be indented at least 2 spaces."
"""

from ettest.testcases.snippets.snippets import Error
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest

def test_rule_2_a_file(manager) -> None:
    """
    File test cases for rule 2a
    """
    tests = [
        FileTest("eastwood-rule-2a.c", [])
    ]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors
