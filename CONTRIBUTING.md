# Developing XOCE Pebble UI

Follow the [shared XOCE contributor conventions](https://github.com/xefensor/xoce/blob/main/docs/contributor-conventions.md)
alongside the library-specific boundaries below.

## Select the development environment

This checkout selects **Node 22.23.4** in `.node-version` and **Python 3.12.15** in
`.python-version`; CI reads those same files. A Node version manager that reads
`.node-version` can select it automatically. With fnm and uv already installed:

```sh
fnm use --install-if-missing
uv venv --python "$(cat .python-version)" .venv
. .venv/bin/activate
npm run check:setup
```

The setup check reports missing or mismatched runtimes and installs nothing.
The virtual environment stays in ignored `.venv/`. Install locked dependencies
with `npm ci --ignore-scripts` after selecting the runtimes.


Use Node 22, Python 3.12 and a C99 compiler (`cc`, or set `CC`). Portable tests do not need a
phone, watch or Pebble SDK. For native SDK builds install [Pebble SDK
4.33.1](https://developer.repebble.com/sdk/) and make `pebble` available on PATH. This
repository builds independently from its own committed dependencies.

```sh
npm ci --ignore-scripts
npm test
npm run build
npm run pack:candidate
```

`npm test` runs source/native, workflow, release-fixture and documentation checks. CI uses Node
22 and Python 3.12; it does not claim SDK, emulator or physical-device coverage.

`src/c` and `include` own drawing helpers and measurements; `src/js/settings-style.js` owns the
shared phone CSS. Apps retain all lifecycle, input, state and controller ownership.

## Outputs and evidence

Active source lives in the directories above. Generated SDK and npm packages live in ignored
`build/candidate/`. Disposable QA belongs in `build/qa/`. Candidate checks reject changed source
or altered build outputs. Frozen `dist/`, `releases/` and root `dist.zip` belong to their
original release tags. Existing `docs/qa/` records and images are historical evidence; do not
overwrite or relabel them.

For visual changes, inspect the consuming app in the emulator; portable tests establish no
visual/device claim. Report source tests, packaged audits, emulator runs and real-device tests
separately.

## Release commands

`npm run release:prepare` validates committed frozen inputs and writes `build/release/`. It
never builds or publishes. `npm run release:publish` is restricted to the manually dispatched
GitHub release workflow on main. See [RELEASING](docs/RELEASING.md) for freezing a new version
and safe reruns. Finish authorized work in reviewable local commits; pushing or publication
needs explicit scope.

[History index](docs/history/index.md).

## Optional consumer pin audit

`python3 scripts/check-consumers.py` audits the five sibling app checkouts
(Timer, Dashboard, Dynamic, Halftone and Open Activity). Each must already have
its dependencies installed with `npm ci --ignore-scripts`. To check only selected
apps, pass their checkout paths:

```sh
python3 scripts/check-consumers.py ../pebble-app-timer ../halftone-watchface
```

The audit verifies each app's own `vendor/UI.json` checksums, archive package
identity, dependency/lockfile and installed source/SDK bytes. Different consumer
versions and archives are allowed. The JSON report in `build/qa/consumer-pins.json`
separately records whether each package's version and sources match this library
checkout, including `src/`, `include/` and root `.js`/`.h`/`.c` exports. A newer
embedded 0.1.3 snapshot does not validate the standalone 0.1.1
release. Font hashes and cross-consumer equality are informational. The audit
does not build an app or establish runtime, emulator, visual or device behavior.
It is not required for building, testing, packaging or releasing this library;
standalone checkouts need no sibling repositories.
