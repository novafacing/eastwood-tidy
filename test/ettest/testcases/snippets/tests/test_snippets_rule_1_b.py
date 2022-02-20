"""
Snippet tests for Rule1b:
"Variable names should be descriptive and meaningful."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule


def test_rule_1_b(manager) -> None:
    """
    Snippet test cases for rule 1b
    """
    snippets = [
        Snippet("  int a = 0;", global_code="int g_var = 0;")
    ]  # No error for this, but we do dump it -- check that it shows up
    for snip in snippets:
        res = manager.test_snippet(snip, dump=True)
        assert not res.unexpected_errors
        assert not res.unseen_errors
        assert "variable | argc" in res.raw_output
        assert "variable | argv" in res.raw_output
        assert "variable | a" in res.raw_output
        assert "variable | g_var" in res.raw_output
        assert "function | main" in res.raw_output
