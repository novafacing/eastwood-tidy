"""
Snippet tests for Rule8f:
"All relevant files should be explicitly included."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule

def test_rule_8_f(manager) -> None:
    """
    Snippet test cases for rule 8f
    """
    snippets = [
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors