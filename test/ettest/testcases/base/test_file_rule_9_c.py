"""
File tests for Rule9c:
"Pointers deallocated with `free` must be set to `NULL`."
"""

from ettest.testcases.snippets.snippets import Error
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest

def test_rule_9_c_file(manager) -> None:
    """
    File test cases for rule 9c
    """
    tests = [
        FileTest("eastwood-rule-9c.c", [])
    ]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors
