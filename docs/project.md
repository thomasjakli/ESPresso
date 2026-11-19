# Project setup and checks

## Host-side controller tests

Install PlatformIO Core and a host C and C++ toolchain with C++17 support. Run from the repository root without any hardware attached.

```sh
pio test -e native
```

The tests use a fake clock to check startup commands, temperature sample validity and freshness, and fault latching. The [Native tests workflow](../.github/workflows/native-tests.yml) runs the same command on every pull request.

## Markdown linting

With Node.js 22 or later, run these commands from the repository root.

```sh
npm ci
npm run lint:markdown
```

GitHub Actions runs the same commands for pull requests.
