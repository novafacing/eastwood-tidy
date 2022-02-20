"""
Snippet tests for Rule5e:
"Function header comments should have a blank line preceding and following."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule

def test_rule_5_e(manager) -> None:
    """
    Snippet test cases for rule 5e
    """
    snippets = [
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors
