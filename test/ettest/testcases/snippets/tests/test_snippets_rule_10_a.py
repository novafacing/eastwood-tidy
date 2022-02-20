"""
Snippet tests for Rule10a:
"All errors should be directed to stderr."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule

def test_rule_10_a(manager) -> None:
    """
    Snippet test cases for rule 10a
    """
    snippets = [
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors
