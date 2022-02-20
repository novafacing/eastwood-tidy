"""
Snippet tests for Rule1c:
"Constants must be uppercase, contain >= 2 characters, be declared using #define, have () for numbers and \"...\" for strings."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule


def test_rule_1_c(manager) -> None:
    """
    Snippet test cases for rule 1c
    """
    snippets = [
        Snippet("", [Error(Rule.I_C, 0, 9)], "#define ROOM_TEMPERATURE 10"),
        Snippet("", [Error(Rule.I_C, 0, 9)], "#define room_temperature (10)"),
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors
