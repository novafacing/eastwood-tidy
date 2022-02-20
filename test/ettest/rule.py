"""Enumeration of rules for eastwood."""

from enum import Enum
from typing import List, Optional


class Rule(Enum):
    """
    Rule enum for snippet tests
    """

    I_A = (
        (
            "Variable names should be all in lowercase, optionally separated by "
            "underscores."
        ),
        (1, "a"),
    )
    I_B = "Variable names should be descriptive and meaningful.", (1, "b")
    I_C = (
        (
            "Constants must be uppercase, contain >= 2 characters, be declared using "
            "#define, have () for numbers and for strings."
        ),
        (1, "c"),
    )
    I_D = (
        (
            "Global variables must begin with the prefix 'g_' and be located at the "
            "top of the file."
        ),
        (1, "d"),
    )
    II_A = (
        (
            "Lines must be under 80 columns. If it is too long, following lines must "
            "be indented at least 2 spaces."
        ),
        (2, "a"),
    )
    II_B = "Functions should fit on 2 pages (240 lines).", (2, "b")
    III_A = (
        (
            "One space after all structure/flow keywords (if, else, while, do, "
            "switch). One space before all open braces."
        ),
        (3, "a"),
    )
    III_B = (
        (
            "One space before and after all logical and arithmetic operators "
            "(+, -, *, /, ==, >, etc)."
        ),
        (3, "b"),
    )
    III_C = "One space after all internal commas and semicolons.", (3, "c")
    III_D = (
        "Define expressions should be grouped, and have a blank line above and below.",
        (3, "d"),
    )
    IV_A = (
        (
            "Two space indentation must be applied to the entire program. Opening "
            "brace must be on the same line as keyword. Else must be on its own line."
        ),
        (4, "a"),
    )
    IV_B = (
        (
            "Parameters should be on one line unless 80 columns is exceeded, in which "
            "case they should be aligned."
        ),
        (4, "b"),
    )
    IV_C = (
        (
            "The while statement of a do-while loop must be on the same line as the "
            "closing brace."
        ),
        (4, "c"),
    )
    V_A = "Comments should be meaningful and not repeat the obvious.", (5, "a")
    V_B = (
        (
            "Comments must be placed above code except to annotate an if, else, "
            "or case statement."
        ),
        (5, "b"),
    )
    V_C = (
        "Comments must be preceded by either a blank line or an open brace.",
        (5, "c"),
    )
    V_D = (
        (
            "Function names must be commented at the end of each function in the "
            "format /* foo() */."
        ),
        (5, "d"),
    )
    V_E = (
        "Function header comments should have a blank line preceding and following.",
        (5, "e"),
    )
    VI_A = (
        (
            "If multiple logical expressions are used, sub-expressions must be "
            "parenthesized."
        ),
        (6, "a"),
    )
    VII_A = (
        "Header comments must be present for all functions, aligned to left edge.",
        (7, "a"),
    )
    VIII_A = "Every .c file should have an associated .h file.", (8, "a")
    VIII_B = "Header files should end in .h.", (8, "b")
    VIII_C = (
        "All header files should have define guards to prevent multiple inclusion.",
        (8, "c"),
    )
    VIII_D = (
        (
            "All local header files should be descendants of the project; no `.`/`..` "
            "permitted."
        ),
        (8, "d"),
    )
    VIII_E = (
        (
            "Includes should be ordered by corresponding header, global includes, then "
            "local includes with each section in lexicographic order."
        ),
        (8, "e"),
    )
    VIII_F = "All relevant files should be explicitly included.", (8, "f")
    VIII_G = (
        "Only non-local includes should used <...>, and local includes must use '...'.",
        (8, "g"),
    )
    IX_A = "Return values of library functions must be checked.", (9, "a")
    IX_B = "File pointers from `fopen` must be closed with `fclose`.", (9, "b")
    IX_C = "Pointers deallocated with `free` must be set to `NULL`.", (9, "c")
    IX_D = "Range checking should be performed on return values.", (9, "d")
    IX_E = (
        "The appropriate zero should be used for `NULL` (0, 0.0, NULL, '\\0').",
        (9, "e"),
    )
    X_A = "All errors should be directed to stderr.", (10, "a")
    XI_A = "Never use tabs.", (11, "a")
    XI_B = "DOS newlines are prohibited.", (11, "b")
    XI_C = "Do not make multiple assignments in a single expression.", (11, "c")
    XI_D = "Do not use embedded constants.", (11, "d")
    XI_E = "Do not use goto.", (11, "e")
    XII_A = "Do not define more than one variable on a single line.", (12, "a")
    XII_B = (
        "All variables must be initialized when they are defined.",
        (12, "b"),
    )
    XII_C = "Variables should be placed as locally as possible.", (12, "c")

    @classmethod
    def roman_to_int(cls, numeral: str) -> int:
        """
        Get integer from roman numeral.

        :param numeral: Numeral to convert to integer.
        """
        roman = {
            "I": 1,
            "V": 5,
            "X": 10,
            "L": 50,
            "C": 100,
            "D": 500,
            "M": 1000,
            "IV": 4,
            "IX": 9,
            "XL": 40,
            "XC": 90,
            "CD": 400,
            "CM": 900,
        }
        i = 0
        num = 0
        while i < len(numeral):
            if i + 1 < len(numeral) and numeral[i : i + 2] in roman:
                num += roman[numeral[i : i + 2]]
                i += 2
            else:
                num += roman[numeral[i]]
                i += 1
        return num

    @classmethod
    def rulenames(cls, roman: bool = False, lower: bool = False) -> List[str]:
        """
        Get a list of all rules.

        :param roman: Whether to return the rules as roman numerals.
        """

        if roman:
            fmt = lambda k: (
                f"{k.split('_')[0]}."
                f"{k.split('_')[1].lower() if lower else k.split('_')[1]}"
            )
        else:
            fmt = lambda k: (
                f"{cls.roman_to_int(k.split('_')[0])}."
                f"{k.split('_')[1].lower() if lower else k.split('_')[1]}"
            )

        return list(map(fmt, cls.__members__.keys()))

    @classmethod
    def from_pair(cls, num: int, sub: str) -> Optional["Rule"]:
        """
        Convert a pair like (8, a) to the correct rule like VIII_A

        :param num: The rule number
        :param sub: The rule sub-identifier
        """
        for rule in cls.__members__.values():
            val = rule.value[1]
            if val[0] == num and val[1] == sub.lower():
                return rule

        return None
