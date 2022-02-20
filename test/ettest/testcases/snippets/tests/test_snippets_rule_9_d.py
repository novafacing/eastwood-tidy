"""
Snippet tests for Rule9d:
"Range checking should be performed on return values."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule

def test_rule_9_d(manager) -> None:
    """
    Snippet test cases for rule 9d
    """
    snippets = [
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors
