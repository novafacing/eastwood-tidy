"""
Snippet tests for Rule7a:
"Header comments must be present for all functions, aligned to left edge."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule

def test_rule_7_a(manager) -> None:
    """
    Snippet test cases for rule 7a
    """
    snippets = [
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors
