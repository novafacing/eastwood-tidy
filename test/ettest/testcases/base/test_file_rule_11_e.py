"""
File tests for Rule11e:
"Do not use goto."
"""

from ettest.testcases.snippets.snippets import Error
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest

def test_rule_11_e_file(manager) -> None:
    """
    File test cases for rule 11e
    """
    tests = [
        FileTest("eastwood-rule-11e.c", [])
    ]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors
