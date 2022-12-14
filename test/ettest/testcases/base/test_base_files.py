from ettest.snippets import Error
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest
from pytest import mark

# File tests for Rule1a:
# "Variable names should be all in lowercase, optionally separated by underscores."


@mark.rule1
def test_rule_1_a_file(manager) -> None:
    """
    File test cases for rule 1a
    """
    tests = [
        FileTest(
            "eastwood-rule-1a.c",
            [
                Error(Rule.I_A, 12, 5),
                Error(Rule.I_A, 15, 5),
                Error(Rule.I_A, 21, 8),
                Error(Rule.I_A, 37, 5),
            ],
        )
    ]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule1b:
# "Variable names should be descriptive and meaningful."


@mark.rule1
def test_rule_1_b_file(manager) -> None:
    """
    File test cases for rule 1b
    """
    # I.B Doesn't error, it just dumps out variable names
    tests = [FileTest("eastwood-rule-1b.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule1c:
# "Constants must be uppercase, contain >= 2 characters, be declared using #define, have () for numbers and for strings."


@mark.rule1
def test_rule_1_c_file(manager) -> None:
    """
    File test cases for rule 1c
    """
    # Doesn't error for embedded constants, just dumps them out, but it does error out for malformed
    # defines
    tests = [
        FileTest(
            "eastwood-rule-1c.c",
            [Error(Rule.I_C, 11, 26), Error(Rule.I_C, 12, 9), Error(Rule.I_C, 14, 33)],
        )
    ]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule1d:
# "Global variables must begin with the prefix 'g_' and be located at the top of the file."


@mark.rule1
def test_rule_1_d_file(manager) -> None:
    """
    File test cases for rule 1d
    """
    tests = [
        FileTest("eastwood-rule-1d.c", [Error(Rule.I_D, 10, 5), Error(Rule.I_D, 24, 5)])
    ]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule2a:
# "Lines must be under 80 columns. If it is too long, following lines must be indented at least 2 spaces."


@mark.rule2
def test_rule_2_a_file(manager) -> None:
    """
    File test cases for rule 2a
    """
    tests = [
        FileTest(
            "eastwood-rule-2a.c",
            [
                Error(Rule.II_A, 7, 97),
                Error(Rule.II_A, 24, 91),
                Error(Rule.II_A, 31, 3),
                Error(Rule.II_A, 34, 91),
            ],
        )
    ]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule2b:
# "Functions should fit on 2 pages (240 lines)."


@mark.rule2
def test_rule_2_b_file(manager) -> None:
    """
    File test cases for rule 2b
    """
    tests = [FileTest("eastwood-rule-2b.c", [Error(Rule.II_B, 17, 1)])]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule3a:
# "One space after all structure/flow keywords (if, else, while, do, switch). One space before all open braces."


@mark.rule3
def test_rule_3_a_file(manager) -> None:
    """
    File test cases for rule 3a
    """
    tests = [
        FileTest(
            "eastwood-rule-3a.c",
            [
                Error(Rule.III_A, 15, 15),
                Error(Rule.III_A, 20, 10),
                Error(Rule.III_A, 20, 16),
                Error(Rule.III_A, 25, 8),
                Error(Rule.III_A, 29, 30),
                Error(Rule.III_A, 33, 8),
                Error(Rule.III_A, 33, 31),
                Error(Rule.III_A, 37, 6),
                Error(Rule.III_A, 41, 15),
                Error(Rule.III_A, 45, 7),
                Error(Rule.III_A, 45, 16),
                Error(Rule.III_A, 49, 5),
                Error(Rule.III_A, 51, 7),
                Error(Rule.III_A, 56, 13),
                Error(Rule.III_A, 62, 11),
                Error(Rule.III_A, 62, 14),
                Error(Rule.III_A, 68, 9),
            ],
        )
    ]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule3b:
# "One space before and after all logical and arithmetic operators (+, -, *, /, ==, >, etc)."


@mark.rule3
def test_rule_3_b_file(manager) -> None:
    """
    File test cases for rule 3b
    """
    tests = [
        FileTest(
            "eastwood-rule-3b.c",
            [
                Error(Rule.III_B, 15, 13),
                Error(Rule.III_B, 15, 14),
                Error(Rule.III_B, 16, 15),
                Error(Rule.III_B, 17, 13),
                Error(Rule.III_B, 20, 8),
                Error(Rule.III_B, 20, 10),
                Error(Rule.III_B, 22, 11),
                Error(Rule.III_B, 22, 13),
                Error(Rule.III_B, 22, 16),
                Error(Rule.III_B, 22, 18),
                Error(Rule.III_B, 22, 21),
                Error(Rule.III_B, 22, 23),
                Error(Rule.III_B, 24, 18),
                Error(Rule.III_C, 24, 18),
            ],
        )
    ]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule3c:
# "One space after all internal commas and semicolons."


@mark.rule3
def test_rule_3_c_file(manager) -> None:
    """
    File test cases for rule 3c
    """
    tests = [
        FileTest(
            "eastwood-rule-3c.c",
            [
                Error(Rule.III_C, 12, 15),
                Error(Rule.III_C, 16, 20),
                Error(Rule.III_C, 16, 22),
                Error(Rule.III_C, 18, 18),
                Error(Rule.III_C, 18, 25),
            ],
        )
    ]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule3d:
# "Define expressions should be grouped, and have a blank line above and below."


@mark.rule3
def test_rule_3_d_file(manager) -> None:
    """
    File test cases for rule 3d
    """
    tests = [
        FileTest(
            "eastwood-rule-3d.c",
            [
                # Indentation errors
                Error(Rule.III_D, 11, 3),
                Error(Rule.III_D, 15, 3),
                Error(Rule.III_D, 23, 3),
                Error(Rule.III_D, 39, 3),
                # Grouping errors
                Error(Rule.III_D, 22, 1),
                Error(Rule.III_D, 26, 1),
                Error(Rule.III_D, 30, 1),
                Error(Rule.III_D, 41, 1),
            ],
        )
    ]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule3e:
# Never put trailing whitespace at the end of a line


@mark.rule3
def test_rule_3_e_file(manager) -> None:
    """
    File test cases for rule 3e
    """
    tests = [
        FileTest(
            "eastwood-rule-3e.c",
            [
                Error(Rule.III_E, 2, 55),
                Error(Rule.III_E, 11, 13),
                Error(Rule.III_E, 13, 14),
                Error(Rule.III_E, 15, 11),
                Error(Rule.III_E, 17, 16),
                Error(Rule.III_E, 19, 16),
                Error(Rule.III_E, 21, 18),
                Error(Rule.III_E, 23, 19),
                Error(Rule.III_E, 25, 16),
                Error(Rule.III_E, 27, 18),
                Error(Rule.III_E, 29, 52),
                Error(Rule.III_E, 32, 79),
                Error(Rule.III_E, 35, 17),
                Error(Rule.III_E, 37, 19),
                Error(Rule.III_E, 39, 18),
                Error(Rule.III_E, 41, 17),
                Error(Rule.III_E, 43, 10),
                Error(Rule.III_E, 45, 8),
                Error(Rule.III_E, 47, 6),
                Error(Rule.III_E, 49, 4),
            ],
        )
    ]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule4a:
# "Two space indentation must be applied to the entire program. Opening brace must be on the same line as keyword. Else must be on its own line."


@mark.rule4
def test_rule_4_a_file(manager) -> None:
    """
    File test cases for rule 4a
    """
    tests = [
        FileTest(
            "eastwood-rule-4a.c",
            [
                # There are a few things in this file that trigger 3a errors due to
                # not just 1 space between paren and brace, but we want it to also
                # flag for 4a since they are on the next line - we expect these
                Error(Rule.III_A, 26, 10),
                Error(Rule.III_A, 28, 17),
                Error(Rule.III_A, 36, 17),
                Error(Rule.III_A, 40, 5),
                # Rule 4a tests
                Error(Rule.IV_A, 14, 1),
                Error(Rule.IV_A, 15, 5),
                Error(Rule.IV_A, 16, 1),
                Error(Rule.IV_A, 17, 9),
                Error(Rule.IV_A, 18, 3),
                Error(Rule.IV_A, 25, 1),
                Error(Rule.IV_A, 26, 5),
                Error(Rule.IV_A, 27, 5),
                Error(Rule.IV_A, 28, 9),
                Error(Rule.IV_A, 29, 9),
                Error(Rule.IV_A, 30, 13),
                Error(Rule.IV_A, 31, 9),
                Error(Rule.IV_A, 32, 9),
                Error(Rule.IV_A, 33, 9),
                Error(Rule.IV_A, 34, 13),
                Error(Rule.IV_A, 35, 9),
                Error(Rule.IV_A, 36, 5),
                Error(Rule.IV_A, 36, 11),
                Error(Rule.IV_A, 37, 5),
                Error(Rule.IV_A, 38, 9),
                Error(Rule.IV_A, 39, 5),
                Error(Rule.IV_A, 40, 5),
                Error(Rule.IV_A, 41, 9),
                Error(Rule.IV_A, 42, 5),
                Error(Rule.IV_A, 43, 5),
                Error(Rule.IV_A, 44, 7),
                Error(Rule.IV_A, 45, 5),
                Error(Rule.IV_A, 46, 7),
                Error(Rule.IV_A, 47, 9),
                Error(Rule.IV_A, 48, 11),
                Error(Rule.IV_A, 49, 7),
                Error(Rule.IV_A, 50, 7),
                Error(Rule.IV_A, 54, 7),
                Error(Rule.IV_A, 56, 7),
                Error(Rule.IV_A, 57, 7),
                Error(Rule.IV_A, 58, 9),
                Error(Rule.IV_A, 59, 7),
                Error(Rule.IV_A, 60, 5),
            ],
        )
    ]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule4b:
# "Parameters should be on one line unless 80 columns is exceeded, in which case they should be aligned."


@mark.rule4
def test_rule_4_b_file(manager) -> None:
    """
    File test cases for rule 4b
    """
    tests = [
        FileTest(
            "eastwood-rule-4b.c", [Error(Rule.IV_B, 16, 10), Error(Rule.IV_B, 17, 11)]
        )
    ]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule4c:
# "The while statement of a do-while loop must be on the same line as the closing brace."


@mark.rule4
def test_rule_4_c_file(manager) -> None:
    """
    File test cases for rule 4c
    """
    tests = [FileTest("eastwood-rule-4c.c", [Error(Rule.IV_C, 16, 4)])]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule5a:
# "Comments should be meaningful and not repeat the obvious."


@mark.rule5
def test_rule_5_a_file(manager) -> None:
    """
    File test cases for rule 5a
    """
    tests = [FileTest("eastwood-rule-5a.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule5b:
# "Comments must be placed above code except to annotate an if, else, or case statement."


@mark.rule5
def test_rule_5_b_file(manager) -> None:
    """
    File test cases for rule 5b
    """
    tests = [
        FileTest(
            "eastwood-rule-5b.c",
            [
                Error(Rule.V_B, 21, 15),
                Error(Rule.V_B, 24, 33),
                Error(Rule.V_B, 27, 16),
            ],
        )
    ]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule5c:
# "Comments must be preceded by either a blank line or an open brace."


@mark.rule5
def test_rule_5_c_file(manager) -> None:
    """
    File test cases for rule 5c
    """
    tests = [FileTest("eastwood-rule-5c.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule5d:
# "Function names must be commented at the end of each function in the format /* foo() */."


@mark.rule5
def test_rule_5_d_file(manager) -> None:
    """
    File test cases for rule 5d
    """
    tests = [
        FileTest(
            "eastwood-rule-5d.c",
            [
                Error(Rule.V_D, 14, 1),
                Error(Rule.V_D, 22, 3),
                Error(Rule.V_D, 30, 4),
            ],
        )
    ]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule5e:
# "Function header comments should have a blank line preceding and following."


@mark.rule5
def test_rule_5_e_file(manager) -> None:
    """
    File test cases for rule 5e
    """
    tests = [
        FileTest(
            "eastwood-rule-5e.c",
            [
                Error(Rule.V_E, 11, 1),
                Error(Rule.V_E, 13, 1),
            ],
        )
    ]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule6a:
# "If multiple logical expressions are used, sub-expressions must be parenthesized."


@mark.rule6
def test_rule_6_a_file(manager) -> None:
    """
    File test cases for rule 6a
    """
    tests = [
        FileTest(
            "eastwood-rule-6a.c",
            [
                Error(Rule.VI_A, 15, 9),
                Error(Rule.VI_A, 15, 20),
                Error(Rule.VI_A, 24, 28),
            ],
        )
    ]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule7a:
# "Header comments must be present for all functions, aligned to left edge."


@mark.rule7
def test_rule_7_a_file(manager) -> None:
    """
    File test cases for rule 7a
    """
    tests = [
        FileTest(
            "eastwood-rule-7a.c",
            [
                # We have to cause these errors to have a missing header comment
                Error(Rule.V_E, 8, 10),
                Error(Rule.V_E, 11, 1),
                Error(Rule.VII_A, 11, 1),
                Error(Rule.VII_A, 16, 1),
            ],
        )
    ]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule8a:
# "Every .c file should have an associated .h file."


@mark.rule8
def test_rule_8_a_file(manager) -> None:
    """
    File test cases for rule 8a
    """
    tests = [FileTest("eastwood-rule-8a.c", [Error(Rule.VIII_A, 6, 1)])]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule8b:
# "Header files should end in .h."


@mark.rule8
def test_rule_8_b_file(manager) -> None:
    """
    File test cases for rule 8b
    """
    tests = [
        FileTest(
            "eastwood-rule-8b.c",
            [
                Error(Rule.VIII_B, 6, 1),
                Error(Rule.VIII_B, 7, 1),
            ],
        )
    ]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule8c:
# "All header files should have define guards to prevent multiple inclusion."


@mark.rule8
def test_rule_8_c_file(manager) -> None:
    """
    File test cases for rule 8c
    """
    tests = [FileTest("eastwood-rule-8c.c", [Error(Rule.VIII_C, 6, 1)])]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule8d:
# "All local header files should be descendants of the project; no `.`/`..` permitted."


@mark.rule8
def test_rule_8_d_file(manager) -> None:
    """
    File test cases for rule 8d
    """
    tests = [
        FileTest(
            "eastwood-rule-8d.c", [Error(Rule.VIII_D, 6, 1), Error(Rule.VIII_D, 7, 1)]
        )
    ]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule8e:
# "Includes should be ordered by corresponding header, global includes, then local includes with each section in lexicographic order."


@mark.rule8
def test_rule_8_e_file(manager) -> None:
    """
    File test cases for rule 8e
    """
    tests = [
        FileTest(
            "eastwood-rule-8e.c",
            [
                Error(Rule.VIII_E, 9, 1),
                Error(Rule.VIII_E, 10, 1),
                Error(Rule.VIII_E, 13, 1),
                Error(Rule.VIII_E, 14, 1),
            ],
        )
    ]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule8f:
# "All relevant files should be explicitly included."


@mark.rule8
def test_rule_8_f_file(manager) -> None:
    """
    File test cases for rule 8f
    """
    tests = [FileTest("eastwood-rule-8f.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule8g:
# "Only non-local includes should used <...>, and local includes must use '...'."


@mark.rule8
def test_rule_8_g_file(manager) -> None:
    """
    File test cases for rule 8g
    """
    tests = [
        FileTest(
            "eastwood-rule-8g.c",
            [
                Error(Rule.VIII_C, 8, 1),
                Error(Rule.VIII_G, 7, 10),
                Error(Rule.VIII_G, 8, 10),
            ],
        )
    ]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule9a:
# "Return values of library functions must be checked."


@mark.rule9
def test_rule_9_a_file(manager) -> None:
    """
    File test cases for rule 9a
    """
    tests = [FileTest("eastwood-rule-9a.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule9b:
# "File pointers """


@mark.rule9
def test_rule_9_b_file(manager) -> None:
    """
    File test cases for rule 9b
    """
    tests = [FileTest("eastwood-rule-9b.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule9c:
# "Pointers deallocated with `free` must be set to `NULL`."


@mark.rule9
def test_rule_9_c_file(manager) -> None:
    """
    File test cases for rule 9c
    """
    tests = [FileTest("eastwood-rule-9c.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule9d:
# "Range checking should be performed on return values."


@mark.rule9
def test_rule_9_d_file(manager) -> None:
    """
    File test cases for rule 9d
    """
    tests = [FileTest("eastwood-rule-9d.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule9e:
# "The appropriate zero should be used for `NULL` (0, 0.0, NULL, '\0')."


@mark.rule9
def test_rule_9_e_file(manager) -> None:
    """
    File test cases for rule 9e
    """
    tests = [FileTest("eastwood-rule-9e.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule10a:
# "All errors should be directed to stderr."


@mark.rule10
def test_rule_10_a_file(manager) -> None:
    """
    File test cases for rule 10a
    """
    tests = [FileTest("eastwood-rule-10a.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule11a:
# "Never use tabs."


@mark.rule11
def test_rule_11_a_file(manager) -> None:
    """
    File test cases for rule 11a
    """
    tests = [
        FileTest(
            "eastwood-rule-11a.c", [Error(Rule.IV_A, 13, 2), Error(Rule.XI_A, 13, 2)]
        )
    ]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule11b:
# "DOS newlines are prohibited."


@mark.rule11
def test_rule_11_b_file(manager) -> None:
    """
    File test cases for rule 11b
    """
    tests = [FileTest("eastwood-rule-11b.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule11c:
# "Do not make multiple assignments in a single expression."


@mark.rule11
def test_rule_11_c_file(manager) -> None:
    """
    File test cases for rule 11c
    """
    tests = [
        FileTest(
            "eastwood-rule-11c.c",
            [
                Error(Rule.XI_C, 15, 7),
                Error(Rule.XI_C, 19, 10),
                Error(Rule.XI_C, 23, 7),
                Error(Rule.XI_C, 25, 3),
            ],
        )
    ]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule11d:
# "Do not use embedded constants."


@mark.rule11
def test_rule_11_d_file(manager) -> None:
    """
    File test cases for rule 11d
    """
    tests = [FileTest("eastwood-rule-11d.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule11e:
# "Do not use goto."


@mark.rule11
def test_rule_11_e_file(manager) -> None:
    """
    File test cases for rule 11e
    """
    tests = [
        FileTest(
            "eastwood-rule-11e.c",
            [
                Error(Rule.XI_E, 12, 3),
            ],
        )
    ]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule12a:
# "Do not define more than one variable on a single line."


@mark.rule12
def test_rule_12_a_file(manager) -> None:
    """
    File test cases for rule 12a
    """
    tests = [
        FileTest(
            "eastwood-rule-12a.c",
            [
                Error(Rule.XII_A, 12, 16),
                Error(Rule.XII_A, 19, 15),
                Error(Rule.XII_B, 20, 7),
                Error(Rule.XII_A, 20, 10),
                Error(Rule.XII_B, 20, 10),
            ],
        )
    ]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule12b:
# "All variables must be initialized when they are defined."


@mark.rule12
def test_rule_12_b_file(manager) -> None:
    """
    File test cases for rule 12b
    """
    tests = [
        FileTest(
            "eastwood-rule-12b.c",
            [
                Error(Rule.XII_B, 8, 5),
                Error(Rule.XII_B, 17, 7),
            ],
        )
    ]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"


# File tests for Rule12c:
# "Variables should be placed as locally as possible."


@mark.rule12
def test_rule_12_c_file(manager) -> None:
    """
    File test cases for rule 12c
    """
    tests = [FileTest("eastwood-rule-12c.c", [])]
    for test in tests:
        res = manager.test_file(test)
        assert (
            not res.unexpected_errors
        ), f"Received UNEXPECTED error(s): {res.unexpected_errors}"
        assert (
            not res.unseen_errors
        ), f"Did not receive EXPECTED error(s): {res.unseen_errors}"
