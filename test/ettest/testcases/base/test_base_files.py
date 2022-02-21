from ettest.testcases.snippets.snippets import Error
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest

"""
File tests for Rule1a:
"Variable names should be all in lowercase, optionally separated by underscores."
"""


def test_rule_1_a_file(manager) -> None:
    """
    File test cases for rule 1a
    """
    tests = [
        FileTest("eastwood-rule-1a.c", [Error(Rule.I_A, 11, 5), Error(Rule.I_A, 13, 5)])
    ]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule1b:
"Variable names should be descriptive and meaningful."
"""


def test_rule_1_b_file(manager) -> None:
    """
    File test cases for rule 1b
    """
    tests = [FileTest("eastwood-rule-1b.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule1c:
"Constants must be uppercase, contain >= 2 characters, be declared using #define, have () for numbers and for strings."
"""


def test_rule_1_c_file(manager) -> None:
    """
    File test cases for rule 1c
    """
    tests = [FileTest("eastwood-rule-1c.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule1d:
"Global variables must begin with the prefix 'g_' and be located at the top of the file."
"""


def test_rule_1_d_file(manager) -> None:
    """
    File test cases for rule 1d
    """
    tests = [FileTest("eastwood-rule-1d.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule2a:
"Lines must be under 80 columns. If it is too long, following lines must be indented at least 2 spaces."
"""


def test_rule_2_a_file(manager) -> None:
    """
    File test cases for rule 2a
    """
    tests = [FileTest("eastwood-rule-2a.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule2b:
"Functions should fit on 2 pages (240 lines)."
"""


def test_rule_2_b_file(manager) -> None:
    """
    File test cases for rule 2b
    """
    tests = [FileTest("eastwood-rule-2b.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule3a:
"One space after all structure/flow keywords (if, else, while, do, switch). One space before all open braces."
"""


def test_rule_3_a_file(manager) -> None:
    """
    File test cases for rule 3a
    """
    tests = [FileTest("eastwood-rule-3a.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule3b:
"One space before and after all logical and arithmetic operators (+, -, *, /, ==, >, etc)."
"""


def test_rule_3_b_file(manager) -> None:
    """
    File test cases for rule 3b
    """
    tests = [FileTest("eastwood-rule-3b.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule3c:
"One space after all internal commas and semicolons."
"""


def test_rule_3_c_file(manager) -> None:
    """
    File test cases for rule 3c
    """
    tests = [FileTest("eastwood-rule-3c.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule3d:
"Define expressions should be grouped, and have a blank line above and below."
"""


def test_rule_3_d_file(manager) -> None:
    """
    File test cases for rule 3d
    """
    tests = [FileTest("eastwood-rule-3d.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule4a:
"Two space indentation must be applied to the entire program. Opening brace must be on the same line as keyword. Else must be on its own line."
"""


def test_rule_4_a_file(manager) -> None:
    """
    File test cases for rule 4a
    """
    tests = [FileTest("eastwood-rule-4a.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule4b:
"Parameters should be on one line unless 80 columns is exceeded, in which case they should be aligned."
"""


def test_rule_4_b_file(manager) -> None:
    """
    File test cases for rule 4b
    """
    tests = [FileTest("eastwood-rule-4b.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule4c:
"The while statement of a do-while loop must be on the same line as the closing brace."
"""


def test_rule_4_c_file(manager) -> None:
    """
    File test cases for rule 4c
    """
    tests = [FileTest("eastwood-rule-4c.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule5a:
"Comments should be meaningful and not repeat the obvious."
"""


def test_rule_5_a_file(manager) -> None:
    """
    File test cases for rule 5a
    """
    tests = [FileTest("eastwood-rule-5a.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule5b:
"Comments must be placed above code except to annotate an if, else, or case statement."
"""


def test_rule_5_b_file(manager) -> None:
    """
    File test cases for rule 5b
    """
    tests = [FileTest("eastwood-rule-5b.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule5c:
"Comments must be preceded by either a blank line or an open brace."
"""


def test_rule_5_c_file(manager) -> None:
    """
    File test cases for rule 5c
    """
    tests = [FileTest("eastwood-rule-5c.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule5d:
"Function names must be commented at the end of each function in the format /* foo() */."
"""


def test_rule_5_d_file(manager) -> None:
    """
    File test cases for rule 5d
    """
    tests = [FileTest("eastwood-rule-5d.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule5e:
"Function header comments should have a blank line preceding and following."
"""


def test_rule_5_e_file(manager) -> None:
    """
    File test cases for rule 5e
    """
    tests = [FileTest("eastwood-rule-5e.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule6a:
"If multiple logical expressions are used, sub-expressions must be parenthesized."
"""


def test_rule_6_a_file(manager) -> None:
    """
    File test cases for rule 6a
    """
    tests = [FileTest("eastwood-rule-6a.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule7a:
"Header comments must be present for all functions, aligned to left edge."
"""


def test_rule_7_a_file(manager) -> None:
    """
    File test cases for rule 7a
    """
    tests = [FileTest("eastwood-rule-7a.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule8a:
"Every .c file should have an associated .h file."
"""


def test_rule_8_a_file(manager) -> None:
    """
    File test cases for rule 8a
    """
    tests = [FileTest("eastwood-rule-8a.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule8b:
"Header files should end in .h."
"""


def test_rule_8_b_file(manager) -> None:
    """
    File test cases for rule 8b
    """
    tests = [FileTest("eastwood-rule-8b.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule8c:
"All header files should have define guards to prevent multiple inclusion."
"""


def test_rule_8_c_file(manager) -> None:
    """
    File test cases for rule 8c
    """
    tests = [FileTest("eastwood-rule-8c.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule8d:
"All local header files should be descendants of the project; no `.`/`..` permitted."
"""


def test_rule_8_d_file(manager) -> None:
    """
    File test cases for rule 8d
    """
    tests = [FileTest("eastwood-rule-8d.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule8e:
"Includes should be ordered by corresponding header, global includes, then local includes with each section in lexicographic order."
"""


def test_rule_8_e_file(manager) -> None:
    """
    File test cases for rule 8e
    """
    tests = [FileTest("eastwood-rule-8e.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule8f:
"All relevant files should be explicitly included."
"""


def test_rule_8_f_file(manager) -> None:
    """
    File test cases for rule 8f
    """
    tests = [FileTest("eastwood-rule-8f.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule8g:
"Only non-local includes should used <...>, and local includes must use '...'."
"""


def test_rule_8_g_file(manager) -> None:
    """
    File test cases for rule 8g
    """
    tests = [FileTest("eastwood-rule-8g.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule9a:
"Return values of library functions must be checked."
"""


def test_rule_9_a_file(manager) -> None:
    """
    File test cases for rule 9a
    """
    tests = [FileTest("eastwood-rule-9a.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule9b:
"File pointers """


def test_rule_9_b_file(manager) -> None:
    """
    File test cases for rule 9b
    """
    tests = [FileTest("eastwood-rule-9b.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule9c:
"Pointers deallocated with `free` must be set to `NULL`."
"""


def test_rule_9_c_file(manager) -> None:
    """
    File test cases for rule 9c
    """
    tests = [FileTest("eastwood-rule-9c.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule9d:
"Range checking should be performed on return values."
"""


def test_rule_9_d_file(manager) -> None:
    """
    File test cases for rule 9d
    """
    tests = [FileTest("eastwood-rule-9d.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule9e:
"The appropriate zero should be used for `NULL` (0, 0.0, NULL, '\0')."
"""


def test_rule_9_e_file(manager) -> None:
    """
    File test cases for rule 9e
    """
    tests = [FileTest("eastwood-rule-9e.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule10a:
"All errors should be directed to stderr."
"""


def test_rule_10_a_file(manager) -> None:
    """
    File test cases for rule 10a
    """
    tests = [FileTest("eastwood-rule-10a.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule11a:
"Never use tabs."
"""


def test_rule_11_a_file(manager) -> None:
    """
    File test cases for rule 11a
    """
    tests = [FileTest("eastwood-rule-11a.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule11b:
"DOS newlines are prohibited."
"""


def test_rule_11_b_file(manager) -> None:
    """
    File test cases for rule 11b
    """
    tests = [FileTest("eastwood-rule-11b.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule11c:
"Do not make multiple assignments in a single expression."
"""


def test_rule_11_c_file(manager) -> None:
    """
    File test cases for rule 11c
    """
    tests = [FileTest("eastwood-rule-11c.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule11d:
"Do not use embedded constants."
"""


def test_rule_11_d_file(manager) -> None:
    """
    File test cases for rule 11d
    """
    tests = [FileTest("eastwood-rule-11d.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule11e:
"Do not use goto."
"""


def test_rule_11_e_file(manager) -> None:
    """
    File test cases for rule 11e
    """
    tests = [FileTest("eastwood-rule-11e.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule12a:
"Do not define more than one variable on a single line."
"""


def test_rule_12_a_file(manager) -> None:
    """
    File test cases for rule 12a
    """
    tests = [FileTest("eastwood-rule-12a.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule12b:
"All variables must be initialized when they are defined."
"""


def test_rule_12_b_file(manager) -> None:
    """
    File test cases for rule 12b
    """
    tests = [FileTest("eastwood-rule-12b.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors


"""
File tests for Rule12c:
"Variables should be placed as locally as possible."
"""


def test_rule_12_c_file(manager) -> None:
    """
    File test cases for rule 12c
    """
    tests = [FileTest("eastwood-rule-12c.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert not res.unexpected_errors
        assert not res.unseen_errors
