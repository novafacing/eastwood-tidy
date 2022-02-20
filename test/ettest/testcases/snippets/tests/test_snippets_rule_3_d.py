"""
Snippet tests for Rule3d:
"Define expressions should be grouped, and have a blank line above and below."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule

def test_rule_3_d(manager) -> None:
    """
    Snippet test cases for rule 3d
    """
    snippets = [
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors
