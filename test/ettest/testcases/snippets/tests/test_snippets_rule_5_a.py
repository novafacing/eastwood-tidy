"""
Snippet tests for Rule5a:
"Comments should be meaningful and not repeat the obvious."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule

def test_rule_5_a(manager) -> None:
    """
    Snippet test cases for rule 5a
    """
    snippets = [
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors
