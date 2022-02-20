"""
Snippet tests for Rule2b:
"Functions should fit on 2 pages (240 lines)."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule

def test_rule_2_b(manager) -> None:
    """
    Snippet test cases for rule 2b
    """
    snippets = [
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors
