"""
File tests for Rule8e:
"Includes should be ordered by corresponding header, global includes, then local includes with each section in lexicographic order."
"""

from ettest.testcases.snippets.snippets import Error
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest

def test_rule_8_e_file(manager) -> None:
    """
    File test cases for rule 8e
    """
    tests = [
        FileTest("eastwood-rule-8e.c", [])
    ]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors
