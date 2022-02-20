"""
Snippet tests for Rule12b:
"All variables must be initialized when they are defined."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule

def test_rule_12_b(manager) -> None:
    """
    Snippet test cases for rule 12b
    """
    snippets = [
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors
