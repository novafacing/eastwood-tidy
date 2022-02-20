"""
Snippet tests for Rule1a:
"Variable names should be all in lowercase, optionally separated by underscores."
"""

from ettest.testcases.snippets.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule


def test_rule_1_a(manager) -> None:
    """
    Snippet test cases for rule 1a
    """
    snippets = [
        Snippet("  int room_temperature = 0;"),
        Snippet("  int temperature = 0;"),
        Snippet("  int room_Temperature = 0;", [Error(Rule.I_A, 0, 7)]),
        Snippet("  int RoomTemperature = 0;", [Error(Rule.I_A, 0, 7)]),
        Snippet("  int Room_Temperature = 0;", [Error(Rule.I_A, 0, 7)]),
        Snippet("  int Room_temperature = 0;", [Error(Rule.I_A, 0, 7)]),
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors
