"""File test configurations for ettest"""

from dataclasses import dataclass, field
from typing import List, Optional
from ettest.testcases.snippets.snippets import Error


@dataclass
class FileTest:
    """
    Test over a C file for ettest
    """

    testfile: str  # The filename like "test_rule_1_a.c"
    errors: Optional[List[Error]] = field(default_factory=list)  # The expected errors
