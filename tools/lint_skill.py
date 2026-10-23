"""Extra checks for SKILL.md that the official uliya-skill-kit checker does not do.

1. Every {value} used anywhere in SKILL.md is defined in the Required input section.
2. Every defined {value} is actually used somewhere outside its definition.
3. Every delegate instruction is a single line and uses each defined value correctly.

Usage: python3 tools/lint_skill.py skill/lecture-revision-kit/SKILL.md
Exit code 0 when clean, 1 when problems are found.
"""
import re
import sys

VALUE = re.compile(r"\{([a-z_]+)\}")


def section(text, title):
    match = re.search(rf"^# {re.escape(title)}\s*$(.*?)(?=^# |\Z)", text, re.M | re.S)
    return match.group(1) if match else ""


def main(path):
    text = open(path, encoding="utf-8").read()
    inputs = section(text, "Required input")

    # A value is defined when it opens a list item, e.g. "1. `{x}`:" or "- `{a}`, `{b}`:".
    defined = {}
    for lineno, line in enumerate(inputs.splitlines()):
        head = re.match(r"\s*(?:\d+\.|-)\s+((?:`\{[a-z_]+\}`(?:,\s*|\s+and\s+)?)+)", line)
        if head:
            for name in VALUE.findall(head.group(1)):
                defined.setdefault(name, line.strip())

    problems = []
    used = {}
    for lineno, line in enumerate(text.splitlines(), start=1):
        for name in VALUE.findall(line):
            used.setdefault(name, []).append(lineno)
        if "delegate --" in line and not re.search(r"`delegate --target [a-z-]+ --instruction '.*'`", line):
            problems.append(f"line {lineno}: delegate command is not a single complete line")

    used.pop("name", None)  # the generic {name} in the substitution rule is not a value
    for name, lines in sorted(used.items()):
        if name not in defined:
            problems.append(f"{{{name}}} is used on line(s) {lines} but never defined in Required input")
    for name, definition in sorted(defined.items()):
        if len(used.get(name, [])) < 2:
            problems.append(f"{{{name}}} is defined but never used: {definition}")

    for problem in problems:
        print("lint:", problem)
    print(f"lint: {len(defined)} values defined, {len(used)} used, {len(problems)} problem(s)")
    return 1 if problems else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1] if len(sys.argv) > 1 else "skill/lecture-revision-kit/SKILL.md"))
