"""
File tests for Rule4c:
"The while statement of a do-while loop must be on the same line as the closing brace."
"""

from ettest.testcases.snippets.snippets import Error
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest

def test_rule_4_c_file(manager) -> None:
    """
    File test cases for rule 4c
    """
    tests = [
        FileTest("eastwood-rule-4c.c", [])
    ]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors
