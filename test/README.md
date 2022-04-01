# Clang Tidy Module Tests

Running tests is handled using `pytest` instead of `gtest` or the 
[bespoke method](https://github.com/llvm/llvm-project/blob/main/clang-tools-extra/test/clang-tidy/check_clang_tidy.py)
provided by `LLVM`.

## Configuring Tests

First, configure the `config.json` file in this directory to point to your built
`clang-tidy` binary. Since we are out of tree, this is unfortunately, required.

    Next, you will need to run `poetry install` and `poetry shell` to activate the python
environment for the tests. If you don't have `poetry`, you can install it 
[`here`](https://python-poetry.org/blog/announcing-poetry-1.2.0a1/).

## Running Tests

Tests are run normally using `pytest`. Any normal pytest options apply, eg.
`pytest -k test_rule_1_a_file -s`.

### Running Specific Tests

Tests are marked by the rule they test -- for example tests the exercise the
Rule 1A checks are marked with `@mark.rule1`. This lets you run:

```pytest -m rule1 -v```

To only run tests that exercise that rule. This makes it a little faster and
easier to only run things tests on the code you are actively changing.

## Errors

Errors in the module tests are specified using the [`Error`](ettest/snippets.py)
dataclass. They are simple objects that specify:

* What rule was violated
* What line it occurred on
* What column it occurred on
* Optionally, the message and severity

Errors are used in both file and snippet tests to specify the expected set of errors
that should occur. For example, if we have:

```c
int main() {
    int a = 0;
}
```

We would expect a Rule 4a violation on line 2, because it is indented too far. This
would correspond to `Error(Rule.IV_A, 2, 4)`.

## File Tests

File tests are "classic" tests that run `clang-tidy` against a C file and make assertions
over the reported errors. File tests look like:

```python
from ettest.snippets
from ettest.fixtures import manager
from ettest.rule import Rule
from ettest.filetest import FileTest

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
```


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

Note that for Snippet tests, line numbers are offsets into the provided code, NOT the
offset from the top of the template file the code is inserted into.

For example:

```python
"""
Snippet tests for Rule1a:
"Variable names should be all in lowercase, optionally separated by underscores."
"""

from ettest.snippets import Snippet, Error
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