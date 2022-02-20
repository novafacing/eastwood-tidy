"""
File tests for Rule8g:
"Only non-local includes should used <...>, and local includes must use '...'."
"""

from ettest.testcases.snippets.snippets import Error
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest

def test_rule_8_g_file(manager) -> None:
    """
    File test cases for rule 8g
    """
    tests = [
        FileTest("eastwood-rule-8g.c", [])
    ]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors
