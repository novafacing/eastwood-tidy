"""
Snippet tests for Rule9b:
"File pointers from `fopen` must be closed with `fclose`."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule

def test_rule_9_b(manager) -> None:
    """
    Snippet test cases for rule 9b
    """
    snippets = [
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors
