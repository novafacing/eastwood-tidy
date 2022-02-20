"""
Snippet tests for Rule4b:
"Parameters should be on one line unless 80 columns is exceeded, in which case they should be aligned."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule

def test_rule_4_b(manager) -> None:
    """
    Snippet test cases for rule 4b
    """
    snippets = [
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors
