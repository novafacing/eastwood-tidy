from dataclasses import dataclass, field
from enum import Enum, auto
from typing import Dict, List, Optional, Tuple, Union

from ettest.rule import Rule


@dataclass(eq=True, frozen=True)
class Error:
    """
    Error class for snippet tests
    """

    rule: Rule = field(hash=True)
    line: int = field(hash=True)
    col: Optional[int] = field(hash=True, default=None)
    # Rule that this error is associated with
    # Message that must be in the error output
    msg: Optional[str] = field(hash=False, compare=False, default=None)
    severity: Optional[str] = field(hash=False, compare=False, default=None)

    def __repr__(self) -> str:
        """
        Override repr to easily appear in tb.
        """
        return (
            "Error("
            f"rule={self.rule.name}, line={self.line}, col={self.col}, "
            f"msg={self.msg})"
        )

    def __lt__(self, other: "Error") -> bool:
        """
        Return whether this error is "less than" (before in the file)
        another error to implement total ordering.

        :param other: The other error to compare to
        :return: True if this error is "less than" the other error
        """
        if self.line < other.line:
            return True
        elif self.line == other.line:
            if self.col is None:
                return False
            elif other.col is None:
                return True
            else:
                return self.col < other.col
        else:
            return False


@dataclass
class Snippet:
    """
    Snippet for testing small code samples without writing an entire file.
    """

    # The actual code to use for the snippet. Code will NOT be re-indented or modified in any way.
    code: Union[str, List[str]]

    # List of errors in {(line, col||None): rule} format. Lines can be specified multiple times
    # if more than one error should be raised on the same line.
    errors: List[Error] = field(default_factory=list)
    global_code: Optional[Union[str, List[str]]] = None
