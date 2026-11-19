# Project setup and checks

## Host-side controller tests

Install PlatformIO Core and a host C and C++ toolchain with C++17 support. Run from the repository root without any hardware attached.

```sh
pio test -e native
```

The tests use a fake clock to check startup commands, temperature sample validity and freshness, and fault latching. The [Native tests workflow](../.github/workflows/native-tests.yml) runs the same command on every pull request.

## C and C++ formatting

From the repository root, run these commands with a Python 3 virtual environment active. The [C++ formatting workflow](../.github/workflows/cpp-format.yml) uses the same commands and formatter version on every pull request.

```sh
python -m pip install -r tools/requirements-format.txt
python tools/check_format.py
```

To apply fixes, run `clang-format -i path/to/file.cpp` with the same installed version.

## Markdown linting

With Node.js 22 or later, run these commands from the repository root.

```sh
npm ci
npm run lint:markdown
```

GitHub Actions runs the same commands for pull requests.
