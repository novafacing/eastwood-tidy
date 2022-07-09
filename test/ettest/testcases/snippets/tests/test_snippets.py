from ettest.snippets import Snippet, Error
from ettest.fixtures import manager
from ettest.rule import Rule


def test_rule_1_a(manager) -> None:
    """
    Snippet test cases for rule 1a

    "Variable names should be all in lowercase, optionally separated by underscores."
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


def test_rule_1_b(manager) -> None:
    """
    Snippet test cases for rule 1b

    "Variable names should be descriptive and meaningful."
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


def test_rule_1_c(manager) -> None:
    """
    Snippet test cases for rule 1c
    "Constants must be uppercase, contain >= 2 characters, be declared using #define, have () for numbers and \"...\" for strings."
    """
    snippets = [
        Snippet("", [], "#define ROOM_TEMPERATURE (10)"),
        Snippet("", [Error(Rule.I_C, 0, 9)], "#define ROOM_TEMPERATURE 10"),
        Snippet("", [Error(Rule.I_C, 0, 9)], "#define room_temperature (10)"),
        Snippet("", [Error(Rule.I_C, 0, 9)], "#define R (10)"),
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_1_d(manager) -> None:
    """
    Snippet test cases for rule 1d
    "Global variables must begin with the prefix 'g_' and be located at the top of the file."
    """
    snippets = [
        Snippet("", [Error(Rule.I_D, 0, 5)], "int bad_global = 5;"),
        Snippet(
            "",
            [Error(Rule.I_D, 1, 5)],
            (
                "/*\n"
                " * header\n"
                " */\n"
                "\n"
                "void foo() {\n"
                "} /* foo() */\n"
                "\n"
                "int bad_global = 5;"
            ),
        ),
        Snippet("", [], "int g_good_global = 5;"),
    ]
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_2_a(manager) -> None:
    """
    Snippet test cases for rule 2a
    "Lines must be under 80 columns. If it is too long, following lines must be indented at least 2 spaces."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_2_b(manager) -> None:
    """
    Snippet test cases for rule 2b
    "Functions should fit on 2 pages (240 lines)."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_3_a(manager) -> None:
    """
    Snippet test cases for rule 3a
    "One space after all structure/flow keywords (if, else, while, do, switch). One space before all open braces."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_3_b(manager) -> None:
    """
    Snippet test cases for rule 3b
    "One space before and after all logical and arithmetic operators (+, -, *, /, ==, >, etc)."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_3_c(manager) -> None:
    """
    Snippet test cases for rule 3c
    "One space after all internal commas and semicolons."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_3_d(manager) -> None:
    """
    Snippet test cases for rule 3d
    "Define expressions should be grouped, and have a blank line above and below."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_4_a(manager) -> None:
    """
    Snippet test cases for rule 4a
    "Two space indentation must be applied to the entire program. Opening brace must be on the same line as keyword. Else must be on its own line."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_4_b(manager) -> None:
    """
    Snippet test cases for rule 4b
    "Parameters should be on one line unless 80 columns is exceeded, in which case they should be aligned."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_4_c(manager) -> None:
    """
    Snippet test cases for rule 4c
    "The while statement of a do-while loop must be on the same line as the closing brace."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_5_a(manager) -> None:
    """
    Snippet test cases for rule 5a
    "Comments should be meaningful and not repeat the obvious."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_5_b(manager) -> None:
    """
    Snippet test cases for rule 5b
    "Comments must be placed above code except to annotate an if, else, or case statement."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_5_c(manager) -> None:
    """
    Snippet test cases for rule 5c
    "Comments must be preceded by either a blank line or an open brace."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_5_d(manager) -> None:
    """
    Snippet test cases for rule 5d
    "Function names must be commented at the end of each function in the format /* foo() */."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_5_e(manager) -> None:
    """
    Snippet test cases for rule 5e
    "Function header comments should have a blank line preceding and following."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_6_a(manager) -> None:
    """
    Snippet test cases for rule 6a
    "If multiple logical expressions are used, sub-expressions must be parenthesized."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_7_a(manager) -> None:
    """
    Snippet test cases for rule 7a
    "Header comments must be present for all functions, aligned to left edge."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_8_a(manager) -> None:
    """
    Snippet test cases for rule 8a
    "Every .c file should have an associated .h file."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_8_b(manager) -> None:
    """
    Snippet test cases for rule 8b
    "Header files should end in .h."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_8_c(manager) -> None:
    """
    Snippet test cases for rule 8c
    "All header files should have define guards to prevent multiple inclusion."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_8_d(manager) -> None:
    """
    Snippet test cases for rule 8d
    "All local header files should be descendants of the project; no `.`/`..` permitted."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_8_e(manager) -> None:
    """
    Snippet test cases for rule 8e
    "Includes should be ordered by corresponding header, global includes, then local includes with each section in lexicographic order."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_8_f(manager) -> None:
    """
    Snippet test cases for rule 8f
    "All relevant files should be explicitly included."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_8_g(manager) -> None:
    """
    Snippet test cases for rule 8g
    "Only non-local includes should used <...>, and local includes must use '...'."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_9_a(manager) -> None:
    """
    Snippet test cases for rule 9a
    "Return values of library functions must be checked."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_9_b(manager) -> None:
    """
    Snippet test cases for rule 9b
    "File pointers must be checked for NULLness."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_9_c(manager) -> None:
    """
    Snippet test cases for rule 9c
    "Pointers deallocated with `free` must be set to `NULL`."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_9_d(manager) -> None:
    """
    Snippet test cases for rule 9d
    "Range checking should be performed on return values."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_9_e(manager) -> None:
    """
    Snippet test cases for rule 9e
    "The appropriate zero should be used for `NULL` (0, 0.0, NULL, '\0')."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_10_a(manager) -> None:
    """
    Snippet test cases for rule 10a
    "All errors should be directed to stderr."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_11_a(manager) -> None:
    """
    Snippet test cases for rule 11a
    "Never use tabs."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_11_b(manager) -> None:
    """
    Snippet test cases for rule 11b
    "DOS newlines are prohibited."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_11_c(manager) -> None:
    """
    Snippet test cases for rule 11c
    "Do not make multiple assignments in a single expression."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_11_d(manager) -> None:
    """
    Snippet test cases for rule 11d
    "Do not use embedded constants."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_11_e(manager) -> None:
    """
    Snippet test cases for rule 11e
    "Do not use goto."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_12_a(manager) -> None:
    """
    Snippet test cases for rule 12a
    "Do not define more than one variable on a single line."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_12_b(manager) -> None:
    """
    Snippet test cases for rule 12b
    "All variables must be initialized when they are defined."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors


def test_rule_12_c(manager) -> None:
    """
    Snippet test cases for rule 12c
    "Variables should be placed as locally as possible."
    """
    snippets = []
    for snip in snippets:
        res = manager.test_snippet(snip)
        assert not res.unexpected_errors
        assert not res.unseen_errors
