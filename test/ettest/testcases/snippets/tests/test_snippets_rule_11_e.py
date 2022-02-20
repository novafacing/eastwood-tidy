"""
Snippet tests for Rule11e:
"Do not use goto."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule

def test_rule_11_e(manager) -> None:
    """
    Snippet test cases for rule 11e
    """
    snippets = [
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors