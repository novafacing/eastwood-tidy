"""
Snippet tests for Rule2a:
"Lines must be under 80 columns. If it is too long, following lines must be indented at least 2 spaces."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule

def test_rule_2_a(manager) -> None:
    """
    Snippet test cases for rule 2a
    """
    snippets = [
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors
