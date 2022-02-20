"""
File tests for Rule5b:
"Comments must be placed above code except to annotate an if, else, or case statement."
"""

from ettest.testcases.snippets.snippets import Error
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest

def test_rule_5_b_file(manager) -> None:
    """
    File test cases for rule 5b
    """
    tests = [
        FileTest("eastwood-rule-5b.c", [])
    ]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors
