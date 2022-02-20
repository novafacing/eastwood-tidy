"""
Snippet tests for Rule5b:
"Comments must be placed above code except to annotate an if, else, or case statement."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule

def test_rule_5_b(manager) -> None:
    """
    Snippet test cases for rule 5b
    """
    snippets = [
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors
