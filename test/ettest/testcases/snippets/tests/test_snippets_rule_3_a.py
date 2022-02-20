"""
Snippet tests for Rule3a:
"One space after all structure/flow keywords (if, else, while, do, switch). One space before all open braces."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule

def test_rule_3_a(manager) -> None:
    """
    Snippet test cases for rule 3a
    """
    snippets = [
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors
