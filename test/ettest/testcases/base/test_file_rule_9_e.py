"""
File tests for Rule9e:
"The appropriate zero should be used for `NULL` (0, 0.0, NULL, '\0')."
"""

from ettest.testcases.snippets.snippets import Error
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest

def test_rule_9_e_file(manager) -> None:
    """
    File test cases for rule 9e
    """
    tests = [
        FileTest("eastwood-rule-9e.c", [])
    ]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors
