"""
File tests for Rule8f:
"All relevant files should be explicitly included."
"""

from ettest.testcases.snippets.snippets import Error
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest

def test_rule_8_f_file(manager) -> None:
    """
    File test cases for rule 8f
    """
    tests = [
        FileTest("eastwood-rule-8f.c", [])
    ]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors
