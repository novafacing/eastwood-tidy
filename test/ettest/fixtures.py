"""Test fixtures for eastwood tidy tests"""

from contextlib import contextmanager
from dataclasses import dataclass, field
from json import load
from pathlib import Path
from subprocess import CalledProcessError, CompletedProcess, run
from tempfile import NamedTemporaryFile
from typing import Generator, List, Optional, Union, Tuple, cast
from re import search

from pytest import fixture

from ettest.rule import Rule
from ettest.snippets import Snippet, Error
from ettest.filetest import FileTest


class ErrorCollection(List[Error]):
    """
    Wrapper for a list of Errors that prints nicely in pytest
    """

    # The repr did not print nicely in pytest, but leaving this in case I want to change
    # it in the future
    ...


@dataclass
class TestResult:
    """
    Result of testing an eastwood-tidy testcase.
    """

    expected_errors: ErrorCollection = field(default_factory=ErrorCollection)
    unexpected_errors: ErrorCollection = field(default_factory=ErrorCollection)
    unseen_errors: ErrorCollection = field(default_factory=ErrorCollection)
    raw_output: Optional[str] = None
    debug_result: Optional[CompletedProcess] = None


class TestManager:
    """Test configuration provider."""

    CONFIG_PATH_REL = Path(__file__).with_name("config.json")
    DUMP_OPTS = [
        "{key: eastwood-Rule1bCheck.dump, value: true}",
        "{key: eastwood-Rule11dCheck.dump, value: true}",
    ]
    C_TEMPLATE = (
        '#include "{hdr_name}"\n'
        "\n"
        "#include <stdio.h>\n"
        "#include <stdlib.h>\n"
        "\n"
        "{global_code}"
        "/*\n"
        " * main\n"
        " * This is a test file for testing eastwood-tidy.\n"
        " */\n\n"
        "int main(int argc, char ** argv) {{\n"
        "{code}"
        "}} /* main() */\n"
    )
    DBG_OPT = (
        "--config={CheckOptions: ["
        "{key: eastwood-Rule10aCheck.debug, value: true},"
        "{key: eastwood-Rule11aCheck.debug, value: true},"
        "{key: eastwood-Rule11bCheck.debug, value: true},"
        "{key: eastwood-Rule11cCheck.debug, value: true},"
        "{key: eastwood-Rule11dCheck.debug, value: true},"
        "{key: eastwood-Rule11eCheck.debug, value: true},"
        "{key: eastwood-Rule12aCheck.debug, value: true},"
        "{key: eastwood-Rule12bCheck.debug, value: true},"
        "{key: eastwood-Rule12cCheck.debug, value: true},"
        "{key: eastwood-Rule1aCheck.debug, value: true},"
        "{key: eastwood-Rule1bCheck.debug, value: true},"
        "{key: eastwood-Rule1cCheck.debug, value: true},"
        "{key: eastwood-Rule1dCheck.debug, value: true},"
        "{key: eastwood-Rule2aCheck.debug, value: true},"
        "{key: eastwood-Rule2bCheck.debug, value: true},"
        "{key: eastwood-Rule3aCheck.debug, value: true},"
        "{key: eastwood-Rule3bCheck.debug, value: true},"
        "{key: eastwood-Rule3cCheck.debug, value: true},"
        "{key: eastwood-Rule3dCheck.debug, value: true},"
        "{key: eastwood-Rule3eCheck.debug, value: true},"
        "{key: eastwood-Rule3fCheck.debug, value: true},"
        "{key: eastwood-Rule4aCheck.debug, value: true},"
        "{key: eastwood-Rule4bCheck.debug, value: true},"
        "{key: eastwood-Rule4cCheck.debug, value: true},"
        "{key: eastwood-Rule5aCheck.debug, value: true},"
        "{key: eastwood-Rule5bCheck.debug, value: true},"
        "{key: eastwood-Rule5cCheck.debug, value: true},"
        "{key: eastwood-Rule5dCheck.debug, value: true},"
        "{key: eastwood-Rule5eCheck.debug, value: true},"
        "{key: eastwood-Rule6aCheck.debug, value: true},"
        "{key: eastwood-Rule7aCheck.debug, value: true},"
        "{key: eastwood-Rule8aCheck.debug, value: true},"
        "{key: eastwood-Rule8bCheck.debug, value: true},"
        "{key: eastwood-Rule8cCheck.debug, value: true},"
        "{key: eastwood-Rule8dCheck.debug, value: true},"
        "{key: eastwood-Rule8eCheck.debug, value: true},"
        "{key: eastwood-Rule8fCheck.debug, value: true},"
        "{key: eastwood-Rule8gCheck.debug, value: true},"
        "{key: eastwood-Rule9aCheck.debug, value: true},"
        "{key: eastwood-Rule9bCheck.debug, value: true},"
        "{key: eastwood-Rule9cCheck.debug, value: true},"
        "{key: eastwood-Rule9dCheck.debug, value: true},"
        "{key: eastwood-Rule9eCheck.debug, value: true}]}"
    )

    def __init__(self) -> None:
        """
        Initialize test configuration.
        """

        with self.CONFIG_PATH_REL.open("r") as config_file:
            config = load(config_file)

        self.clang_tidy = Path(config["clang-tidy"]).resolve()
        assert (
            self.clang_tidy.exists()
        ), f"Clang tidy does not exist at {self.clang_tidy}"
        self.testcase_dir = Path(__file__).with_name("testcases")
        self.last_file: Optional[Path] = None

    @classmethod
    def build_config(
        cls, opts: Optional[List[str]] = None, debug: bool = False
    ) -> Optional[List[str]]:
        """
        Build the clang-tidy configuration.

        :param opts: Optional options to add to `CheckOptions` in clang-tidy invocation.
        :param debug: Whether to enable debug mode.
        """
        conf_opts = opts if opts is not None else []

        if debug:
            for rule in Rule.rulenames(roman=False, lower=True):
                conf_opts.append(
                    f"{{key: eastwood-Rule{rule}Check.debug, value: true}}"
                )

        if conf_opts:
            return ["--config", f"{{CheckOptions: [{', '.join(conf_opts)}]}}"]

        return None

    def get_test(self, name: str) -> Optional[Path]:
        """
        Get a test file by name.
        """
        for cfile in self.testcase_dir.rglob("**/*.c"):
            if cfile.name == name or cfile.stem == name:
                return cfile.resolve()
        return None

    def run(
        self,
        sourcefile: Path,
        cwd: Optional[Path] = None,
        opts: Optional[List[str]] = None,
        copts: Optional[List[str]] = None,
        debug: bool = False,
    ) -> Tuple[str, CompletedProcess]:
        """
        Run clang-tidy on source file.

        :param sourcefile: The path to the source file.
        :param cwd: Optionally switch to a different directory to run.
            Defaults to the directory containing the source file.
        :param opts: Optional options to add to `CheckOptions` in clang-tidy invocation.
            These options should look like:
                "{key: eastwood-Rule1bCheck.dump, value: true}"
            Do *not* include the trailing comma!
        :param copts: Optional options to add to the command line invocation.
        """
        cmd = [str(self.clang_tidy.resolve())]
        cmd += ["--checks", "-*,eastwood*"]

        cfg = self.build_config(opts, debug)

        if cfg is not None:
            cmd.extend(cfg)

        if cwd is None:
            cwd = sourcefile.parent

        sourcefile = sourcefile.relative_to(cwd)

        if copts is not None:
            cmd.extend(copts)

        cmd += [str(sourcefile)]
        dcmd = cmd[:]
        dcmd.insert(3, self.DBG_OPT)

        try:
            # Don't check, we return 1 if there are errors :facepalm:
            res = run(cmd, cwd=str(cwd.resolve()), capture_output=True, check=False)

            dres = run(
                dcmd,
                cwd=str(cwd.resolve()),
                capture_output=True,
                check=False,
            )
        except CalledProcessError as e:
            raise Exception(f"Failed to run '{' '.join(cmd)}'") from e

        return res.stdout.decode("utf-8"), dres

    @classmethod
    def make_c(
        cls,
        hdr_name: str,
        code: Union[str, List[str]],
        global_code: Optional[Union[str, List[str]]],
    ) -> str:
        """
        Make a C file from a list of lines.

        :param code: The lines of code.
        """
        if isinstance(code, list):
            code = "\n".join(code) + "\n"
        elif isinstance(code, str):
            code += "\n"
        elif code is None:
            code = "\n"

        if isinstance(global_code, list):
            global_code = "\n".join(global_code) + "\n"
        elif isinstance(global_code, str):
            global_code += "\n\n"
        elif global_code is None:
            global_code = ""

        return cls.C_TEMPLATE.format(
            hdr_name=hdr_name, global_code=global_code, code=code
        )

    @classmethod
    def make_include_guard(cls, filename: str) -> str:
        """
        Generate an include guard for a file.

        :param filename: The stem of the name of the file.
        """
        guard = ""
        guard += f"#ifndef {filename.upper()}_H\n"
        guard += f"#define {filename.upper()}_H\n"
        guard += "\n"
        guard += f"#endif // {filename.upper()}_H\n"
        return guard

    @contextmanager
    def make_code(self, snippet: Snippet) -> Generator[Path, None, None]:
        """
        Make a code file from a snippet.

        :param snippet: The snippet to make a code file from.
        """
        tf = NamedTemporaryFile(mode="w+", suffix=".c", delete=False)
        tfp = Path(tf.name).resolve()
        th = tfp.with_suffix(".h")
        with th.open("w+") as hf:
            hf.write(self.make_include_guard(th.stem))

        code = self.make_c(th.name, snippet.code, snippet.global_code)
        tf.write(code)
        tf.close()

        try:
            yield tfp
        finally:

            if tfp is not None and tfp.exists():
                tfp.unlink()
                th.unlink()

    def run_snippet(
        self, snippet: Snippet, dump: bool = False
    ) -> Tuple[str, CompletedProcess]:
        """
        Run clang-tidy on a snippet.

        :param snippet: The snippet to run.
        """
        with self.make_code(snippet) as tf:
            self.last_file = tf
            res, dres = self.run(tf, opts=self.DUMP_OPTS if dump else None)
            return res, dres

    @classmethod
    def output_to_rule(cls, output: str) -> Rule:
        """
        Parse the output of clang-tidy to determine the rule that was violated.

        :param output: The output of clang-tidy like '[eastwood-Rule8aCheck]'
        """
        reg = r"\[eastwood-Rule([0-9]+)([a-z])Check\]"
        match = search(reg, output)

        if match is None:
            raise Exception(f"Failed to parse output: {output}")

        rule = Rule.from_pair(int(match.group(1)), match.group(2))

        if rule is None:
            raise Exception(f"Failed to parse output: {output}")

        return rule

    def collect_output(
        self, output: str, severity: Optional[str] = None, snippets: bool = False
    ) -> List[Error]:
        """
        Collect warnings from the output of clang-tidy.

        :param output: The output of clang-tidy.
        """
        errs = []
        for line in map(lambda l: l.split(" "), output.splitlines()):
            if line[0].startswith(str(self.last_file.resolve())):
                _, _line_num, _col_num, *_ = line[0].split(":")
                line_num = int(_line_num)
                col_num = int(_col_num)

                if snippets:
                    skip_lines = (
                        self.C_TEMPLATE[: self.C_TEMPLATE.index("{code}")].count("\n")
                        + 1
                    )
                    line_num -= skip_lines

                    if line_num < 0:
                        # This is in global code, so we re-adjust the line number
                        line_num += skip_lines
                        skip_lines = (
                            self.C_TEMPLATE[
                                : self.C_TEMPLATE.index("{global_code}")
                            ].count("\n")
                            + 1
                        )
                        line_num -= skip_lines

                sev = line[1].split(":")[0]
                msg = line[2:-1]
                try:
                    rule = self.output_to_rule(line[-1])
                except Exception:
                    # Failed to parse the output probably due to [clang-diagnostic-error]
                    continue

                if severity is None or severity == sev:
                    errs.append(Error(rule, line_num, col_num, " ".join(msg), sev))
        return errs

    def test_snippet(self, snippet: Snippet, dump: bool = False) -> TestResult:
        """
        Test a snippet.

        :param snippet: The snippet to test.
        """
        res, dres = self.run_snippet(snippet, dump)
        errors = self.collect_output(res, "error", snippets=True)
        warnings = self.collect_output(res, "warning", snippets=True)
        notes = self.collect_output(res, "note", snippets=True)
        all_msgs = set(errors + warnings + notes)
        expected_msgs = set(snippet.errors)
        return TestResult(
            ErrorCollection(sorted(list(all_msgs & expected_msgs))),
            ErrorCollection(sorted(list(all_msgs - expected_msgs))),
            ErrorCollection(sorted(list(expected_msgs - all_msgs))),
            res,
            dres,
        )

    def test_file(self, file_test: FileTest) -> TestResult:
        """
        Test a file.

        :param file_test: The file to test.
        """
        tf = self.get_test(file_test.testfile)
        self.last_file = tf
        assert tf.exists(), f"File does not exist: {tf}"
        res, dres = self.run(cast(Path, tf))
        errors = self.collect_output(res, "error", snippets=False)
        warnings = self.collect_output(res, "warning", snippets=False)
        notes = self.collect_output(res, "note", snippets=False)
        all_msgs = set(errors + warnings + notes)
        expected_msgs = set(file_test.errors)
        return TestResult(
            ErrorCollection(sorted(list(all_msgs & expected_msgs))),
            ErrorCollection(sorted(list(all_msgs - expected_msgs))),
            ErrorCollection(sorted(list(expected_msgs - all_msgs))),
            res,
            dres,
        )


@fixture
def manager() -> TestManager:
    """
    Provide test configuration.
    """
    return TestManager()
