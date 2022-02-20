"""
File tests for Rule3d:
"Define expressions should be grouped, and have a blank line above and below."
"""

from ettest.testcases.snippets.snippets import Error
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest

def test_rule_3_d_file(manager) -> None:
    """
    File test cases for rule 3d
    """
    tests = [
        FileTest("eastwood-rule-3d.c", [])
    ]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors
