"""
Snippet tests for Rule9a:
"Return values of library functions must be checked."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule

def test_rule_9_a(manager) -> None:
    """
    Snippet test cases for rule 9a
    """
    snippets = [
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors
