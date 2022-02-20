"""
Snippet tests for Rule6a:
"If multiple logical expressions are used, sub-expressions must be parenthesized."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule

def test_rule_6_a(manager) -> None:
    """
    Snippet test cases for rule 6a
    """
    snippets = [
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors
