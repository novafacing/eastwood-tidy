# Clang Tidy Module Tests

Running tests is handled using `pytest` instead of `gtest` or the 
[bespoke method](https://github.com/llvm/llvm-project/blob/main/clang-tools-extra/test/clang-tidy/check_clang_tidy.py)
provided by `LLVM`.

## File Tests


## Snippet Tests

Snippet tests allow specifying just a short code block like:

```
#define ROOM_TEMPERATURE 10
```

that should trigger a clang tidy error. Testing is performed via `pytest` and tests
for existing rules are found in `ettest/testcases/snippets/tests`.

Snippets are composed of code, a (possibly empty) list of expected errors, and optionally
global code. Both code and global code can either be a string or a list of strings that
will be inserted into a template test file to avoid boilerplate C code in test files.

For example:

```python
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
```

The above test asserts that the first two lines trigger no error, while the other four
trigger a Rule1A violation on line 0 (there is only one line, anyway), at column 7.

Snippet tests can also contain global code by using the `global_code` parameter.

Any code in `global_code` will be placed in global scope, so it can be used for testing
global variables, functions other than main, etc.

```
Snippet("  int a = 0;", global_code="int g_var = 0;")
```