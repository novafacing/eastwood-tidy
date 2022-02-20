"""
Snippet tests for Rule4c:
"The while statement of a do-while loop must be on the same line as the closing brace."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule

def test_rule_4_c(manager) -> None:
    """
    Snippet test cases for rule 4c
    """
    snippets = [
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors
