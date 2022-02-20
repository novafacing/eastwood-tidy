"""
Snippet tests for Rule12c:
"Variables should be placed as locally as possible."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule

def test_rule_12_c(manager) -> None:
    """
    Snippet test cases for rule 12c
    """
    snippets = [
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors
