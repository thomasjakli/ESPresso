import os
import subprocess
from pathlib import Path


patterns = ("*.c", "*.cc", "*.cpp", "*.cxx", "*.h", "*.hh", "*.hpp", "*.hxx")
list_sources = [
    "git", "ls-files", "-z", "--cached", "--others", "--exclude-standard", "--deduplicate",
    "--", *patterns,
]
paths = os.fsdecode(subprocess.check_output(list_sources)).split("\0")
sources = [path for path in paths if Path(path).is_file()]

if sources:
    result = subprocess.run(["clang-format", "--style=file", "--dry-run", "--Werror", "--", *sources])
    raise SystemExit(result.returncode)
