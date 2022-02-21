"""Functionality tests to make sure we can even run our tests..."""

from ettest.fixtures import manager
from ettest.snippets import Snippet, Error
from ettest.rule import Rule


def test_run_tidy_file(manager) -> None:
    """
    Test that we can run eastwood-tidy tests.
    """
    tf = manager.get_test("test_run_tidy.c")
    assert tf.exists(), f"Test file does not exist: {tf}"
    res = manager.run(tf)
    assert res
    res = manager.run(tf, opts=manager.DUMP_OPTS)
    assert res
    assert "variable | a" in res


def test_run_tidy_snippet(manager) -> None:
    """
    Test that we can run eastwood-tidy tests on a snippet.
    """
    s = Snippet(code="int a = 0;")
    res = manager.run_snippet(s)
    assert res
    assert "Incorrect indentation level." in res


def test_test_tidy_snippet(manager) -> None:
    """
    Test that we can assert eastwood-tidy tests on a snippet.
    """
    s = Snippet(
        code="int a = 0;",
        errors=[Error(Rule.IV_A, 0, 1, "Incorrect indentation level.")],
    )
    res = manager.test_snippet(s)
    assert (
        not res.unexpected_errors
        and not res.unseen_errors
        and len(res.expected_errors) == 1
    )
