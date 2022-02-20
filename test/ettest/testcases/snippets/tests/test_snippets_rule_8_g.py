"""
Snippet tests for Rule8g:
"Only non-local includes should used <...>, and local includes must use '...'."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule

def test_rule_8_g(manager) -> None:
    """
    Snippet test cases for rule 8g
    """
    snippets = [
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors
