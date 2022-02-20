"""
Snippet tests for Rule12a:
"Do not define more than one variable on a single line."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule

def test_rule_12_a(manager) -> None:
    """
    Snippet test cases for rule 12a
    """
    snippets = [
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors
