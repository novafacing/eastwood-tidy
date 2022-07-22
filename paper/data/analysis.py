from csv import reader
from dataclasses import dataclass
from pathlib import Path
from traceback import print_exc


def scale_to_int(s: str) -> int:
    if s == "Strongly Disagree":
        return 1
    elif s == "Disagree":
        return 2
    elif s == "Neutral":
        return 3
    elif s == "Agree":
        return 4
    elif s == "Strongly Agree":
        return 5


@dataclass
class EndOfSemData:

    """
    Timestamp
    Which of the following best describes your experience with coding standards in any programming language?
    Which of the following best describes your experience with linting tools in any programming language?
    The new linter makes it easier to meet the code standard.
    The new linter saves time meeting the code standard.
    The new linter effectively checks whether my code meets the standard.
    The new linter helps me accurately locate code standard violations in my code.
    The new linter helps improve the quality of my code.
    The new linter helps me find bugs in my code.
    The new linter helps me make my code more readable.
    "On a scale of 1 (hardest)  to 5 (easiest) how difficult is it to locate the code causing a violation with the OLD linter?"
    "On a scale of 1 (hardest) to 5 (easiest) how difficult is it to locate the code causing a violation with the NEW linter?"
    "On a scale of 1 (less time) to 5 (more time) how much time did you spend manually checking your code after using the OLD linter?"
    "On a scale of 1 (less time) to 5 (more time) how much time did you spend manually checking your code after using the NEW linter?"
    "On a scale of 1 (the least) to 5 (the most) how much of the code standard did the OLD linter *effectively* check?"
    "On a scale of 1 (the least) to 5 (the most) how much of the code standard did the NEW linter *effectively* check?"
    The Code Standard is well defined.
    Following the code standard improves my code quality.
    I will continue to use the CS240 Code Standard in future C programming after this course.
    I will continue to use some code standard in future C programming after this course.
    "What changes if any should be made to the new linter? Note: don't list bugs here focus on features or changes that would improve the overall experience of using the linter."
    "What feedback if any do you have concerning the Code Standard?"
    Are you in favor of having a code standard in other courses? Why or why not?
    Any other feedback:
    """

    timestamp: str
    prior_code_standard: str
    prior_linting: str
    new_linter_easier_to_meet: int
    new_linter_saves_time: int
    new_linter_effectively_checks: int
    new_linter_accurately_locate_violations: int
    new_linter_improve_code_quality: int
    new_linter_helps_find_bug: int
    new_linter_code_more_readable: int
    old_linter_locate_violation: int
    new_linter_locate_violation: int
    old_linter_manual_check_time: int
    new_linter_manual_check_time: int
    old_linter_check_coverage: int
    new_linter_check_coverage: int
    code_standard_well_defined: int
    code_standard_code_quality: int
    will_continue_to_use_cs240_code_standard: int
    will_continue_to_use_some_code_standard: int
    what_linter_changes: str
    what_code_stadard_changes: str
    in_favor_of_code_standard_in_other_courses: str
    other_feedback: str

    def __post_init__(self) -> None:
        self.new_linter_easier_to_meet = scale_to_int(self.new_linter_easier_to_meet)
        self.new_linter_saves_time = scale_to_int(self.new_linter_saves_time)
        self.new_linter_effectively_checks = scale_to_int(
            self.new_linter_effectively_checks
        )
        self.new_linter_accurately_locate_violations = scale_to_int(
            self.new_linter_accurately_locate_violations
        )
        self.new_linter_improve_code_quality = scale_to_int(
            self.new_linter_improve_code_quality
        )
        self.new_linter_helps_find_bug = scale_to_int(self.new_linter_helps_find_bug)
        self.new_linter_code_more_readable = scale_to_int(
            self.new_linter_code_more_readable
        )
        self.old_linter_locate_violation = int(self.old_linter_locate_violation)
        self.new_linter_locate_violation = int(self.new_linter_locate_violation)
        self.old_linter_manual_check_time = int(self.old_linter_manual_check_time)
        self.new_linter_manual_check_time = int(self.new_linter_manual_check_time)
        self.old_linter_check_coverage = int(self.old_linter_check_coverage)
        self.new_linter_check_coverage = int(self.new_linter_check_coverage)
        self.code_standard_well_defined = scale_to_int(self.code_standard_well_defined)
        self.code_standard_code_quality = scale_to_int(self.code_standard_code_quality)
        self.will_continue_to_use_cs240_code_standard = scale_to_int(
            self.will_continue_to_use_cs240_code_standard
        )
        self.will_continue_to_use_some_code_standard = scale_to_int(
            self.will_continue_to_use_some_code_standard
        )


@dataclass
class MidOfSemData:
    """
    Timestamp
    Which of the following best describes your experience with coding standards in any programming language?
    Which of the following best describes your experience with linting tools in any programming language?
    The linter makes it easier to meet the code standard.
    The linter saves time meeting the code standard.
    The linter effectively checks whether my code meets the standard.
    The linter helps me accurately locate code standard violations in my code.
    The linter helps improve the quality of my code.
    The linter helps me find bugs in my code.
    The linter helps me make my code more readable.
    The Code Standard is well defined.
    Following the code standard improves my code quality.
    I will continue to use the CS240 Code Standard in future C programming after this course.
    I will continue to use some code standard in future C programming after this course.
    "What changes if any should be made to the Linter?"
    "What feedback if any do you have concerning the Code Standard?"
    Are you in favor of having a code standard in other courses? Why or why not?
    Any other feedback:
    """

    timestamp: str
    prior_code_standard: str
    prior_linting: str
    linter_easier_to_meet: int
    linter_saves_time: int
    linter_effectively_checks: int
    linter_accurately_locate_violations: int
    linter_improve_code_quality: int
    linter_helps_find_bug: int
    linter_code_more_readable: int
    code_standard_well_defined: str
    code_standard_code_quality: str
    will_continue_to_use_cs240_code_standard: str
    will_continue_to_use_some_code_standard: str
    what_linter_changes: str
    what_code_stadard_changes: str
    in_favor_of_code_standard_in_other_courses: str
    other_feedback: str

    def __post_init__(self) -> None:
        self.linter_easier_to_meet = scale_to_int(self.linter_easier_to_meet)
        self.linter_saves_time = scale_to_int(self.linter_saves_time)
        self.linter_effectively_checks = scale_to_int(self.linter_effectively_checks)
        self.linter_accurately_locate_violations = scale_to_int(
            self.linter_accurately_locate_violations
        )
        self.linter_improve_code_quality = scale_to_int(
            self.linter_improve_code_quality
        )
        self.linter_helps_find_bug = scale_to_int(self.linter_helps_find_bug)
        self.linter_code_more_readable = scale_to_int(self.linter_code_more_readable)
        self.code_standard_well_defined = scale_to_int(self.code_standard_well_defined)
        self.code_standard_code_quality = scale_to_int(self.code_standard_code_quality)
        self.will_continue_to_use_cs240_code_standard = scale_to_int(
            self.will_continue_to_use_cs240_code_standard
        )
        self.will_continue_to_use_some_code_standard = scale_to_int(
            self.will_continue_to_use_some_code_standard
        )


end_of_sem = reader(
    Path(__file__)
    .with_name(
        "CS240 Linter + Code Standard Survey Results (2021, End of Semester).csv"
    )
    .read_text()
    .splitlines()
)
mid_of_sem = reader(
    Path(__file__)
    .with_name("CS240 Linter + Code Standard Survey Results (2021, Mid-Semester).csv")
    .read_text()
    .splitlines()
)
end = []
for line in end_of_sem:
    try:
        end.append(EndOfSemData(*line))
    except:
        print_exc()
        print(line)
        exit(1)
mid = []
for line in mid_of_sem:
    try:
        mid.append(MidOfSemData(*line))
    except:
        print_exc()
        print(line)
        exit(1)
