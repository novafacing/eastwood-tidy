"""
Snippet tests for Rule3b:
"One space before and after all logical and arithmetic operators (+, -, *, /, ==, >, etc)."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule

def test_rule_3_b(manager) -> None:
    """
    Snippet test cases for rule 3b
    """
    snippets = [
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors
