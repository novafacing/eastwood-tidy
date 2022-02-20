"""
Snippet tests for Rule8d:
"All local header files should be descendants of the project; no `.`/`..` permitted."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule

def test_rule_8_d(manager) -> None:
    """
    Snippet test cases for rule 8d
    """
    snippets = [
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors
