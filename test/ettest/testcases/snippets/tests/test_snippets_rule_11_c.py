"""
Snippet tests for Rule11c:
"Do not make multiple assignments in a single expression."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule

def test_rule_11_c(manager) -> None:
    """
    Snippet test cases for rule 11c
    """
    snippets = [
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors
