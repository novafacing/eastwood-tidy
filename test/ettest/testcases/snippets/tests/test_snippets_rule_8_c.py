"""
Snippet tests for Rule8c:
"All header files should have define guards to prevent multiple inclusion."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule

def test_rule_8_c(manager) -> None:
    """
    Snippet test cases for rule 8c
    """
    snippets = [
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors
