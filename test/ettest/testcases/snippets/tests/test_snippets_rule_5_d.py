"""
Snippet tests for Rule5d:
"Function names must be commented at the end of each function in the format /* foo() */."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule

def test_rule_5_d(manager) -> None:
    """
    Snippet test cases for rule 5d
    """
    snippets = [
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors
