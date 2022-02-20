"""
Snippet tests for Rule8a:
"Every .c file should have an associated .h file."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule

def test_rule_8_a(manager) -> None:
    """
    Snippet test cases for rule 8a
    """
    snippets = [
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors