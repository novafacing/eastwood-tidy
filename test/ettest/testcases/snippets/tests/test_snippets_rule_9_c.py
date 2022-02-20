"""
Snippet tests for Rule9c:
"Pointers deallocated with `free` must be set to `NULL`."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule

def test_rule_9_c(manager) -> None:
    """
    Snippet test cases for rule 9c
    """
    snippets = [
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors
