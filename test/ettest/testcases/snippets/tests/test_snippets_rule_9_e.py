"""
Snippet tests for Rule9e:
"The appropriate zero should be used for `NULL` (0, 0.0, NULL, '\0')."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule

def test_rule_9_e(manager) -> None:
    """
    Snippet test cases for rule 9e
    """
    snippets = [
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors
