"""
Snippet tests for Rule1d:
"Global variables must begin with the prefix 'g_' and be located at the top of the file."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule

def test_rule_1_d(manager) -> None:
    """
    Snippet test cases for rule 1d
    """
    snippets = [
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors
