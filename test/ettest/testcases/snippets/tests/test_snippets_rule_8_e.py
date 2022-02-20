"""
Snippet tests for Rule8e:
"Includes should be ordered by corresponding header, global includes, then local includes with each section in lexicographic order."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule

def test_rule_8_e(manager) -> None:
    """
    Snippet test cases for rule 8e
    """
    snippets = [
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors
