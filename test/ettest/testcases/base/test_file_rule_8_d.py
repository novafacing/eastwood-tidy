"""
File tests for Rule8d:
"All local header files should be descendants of the project; no `.`/`..` permitted."
"""

from ettest.testcases.snippets.snippets import Error
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest

def test_rule_8_d_file(manager) -> None:
    """
    File test cases for rule 8d
    """
    tests = [
        FileTest("eastwood-rule-8d.c", [])
    ]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors
