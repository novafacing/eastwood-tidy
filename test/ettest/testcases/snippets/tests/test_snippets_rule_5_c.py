"""
Snippet tests for Rule5c:
"Comments must be preceded by either a blank line or an open brace."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule

def test_rule_5_c(manager) -> None:
    """
    Snippet test cases for rule 5c
    """
    snippets = [
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors
