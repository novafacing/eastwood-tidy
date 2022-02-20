"""
Snippet tests for Rule4a:
"Two space indentation must be applied to the entire program. Opening brace must be on the same line as keyword. Else must be on its own line."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule

def test_rule_4_a(manager) -> None:
    """
    Snippet test cases for rule 4a
    """
    snippets = [
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors
