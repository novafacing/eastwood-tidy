"""
Snippet tests for Rule3c:
"One space after all internal commas and semicolons."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule

def test_rule_3_c(manager) -> None:
    """
    Snippet test cases for rule 3c
    """
    snippets = [
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors
