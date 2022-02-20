"""
File tests for Rule4a:
"Two space indentation must be applied to the entire program. Opening brace must be on the same line as keyword. Else must be on its own line."
"""

from ettest.testcases.snippets.snippets import Error
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest

def test_rule_4_a_file(manager) -> None:
    """
    File test cases for rule 4a
    """
    tests = [
        FileTest("eastwood-rule-4a.c", [])
    ]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors
