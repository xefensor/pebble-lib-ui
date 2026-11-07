# XOCE Pebble UI

Small native drawing helpers and a phone settings stylesheet shared by XOCE Pebble apps.

## Download

[Version 0.1.1](https://github.com/xefensor/pebble-lib-ui/releases/tag/v0.1.1) is the current published library.

[package](https://github.com/xefensor/pebble-lib-ui/releases/download/v0.1.1/pebble-xoce-ui-0.1.1.tgz),
[SDK archive](https://github.com/xefensor/pebble-lib-ui/releases/download/v0.1.1/dist.zip), and
[checksums](https://github.com/xefensor/pebble-lib-ui/releases/download/v0.1.1/SHA256SUMS.txt).

Package name: `pebble-xoce-ui`. This library is bundled by a host app; it is not installed on
the watch separately.

Download the package into your project’s `vendor/` directory, then pin it locally:

```sh
npm install --save-exact ./vendor/pebble-xoce-ui-0.1.1.tgz
```

Commit the lockfile and upstream checksum/provenance record. Consumer upgrades are independent.

```c
#include <pebble-xoce-ui/pebble-xoce-ui.h>
```

```js
var commonStyle = require('pebble-xoce-ui/settings-style');
```

The package supplies type-role measurements, glyph centering, canonical vector icons,
full-height action rails, image fitting and button feedback. Hosts own windows, callbacks,
input, timers, font loading, alarms, settings and storage.

Content-only icon entrypoints let watchfaces avoid linking action-control helpers. Timer keeps
its rigid 120 ms menu motion; Dashboard keeps immediate selection.

## Compatibility

The current SDK target is **Pebble Time 2 / Emery**, using **Pebble SDK 4.33.1**.

## Version 0.1.1

- First GitHub release of the shared native drawing helpers and phone settings stylesheet.
- Includes readable type roles, measured glyph centering, canonical icons, full-height action rails, image fitting and stock Pebble action-bar feedback.
- Timer retains its rigid 120 ms selection; Dashboard retains immediate selection. Apps keep ownership of input, state, timers and storage.
- Built with Pebble SDK 4.33.1 for Emery. Validated with portable tests, package checks and consuming-app emulator captures; physical-device and fresh browser testing remain unverified.

## Development

Use Node 22, Python 3.12 and a C compiler. SDK builds additionally need Pebble SDK 4.33.1 on
PATH.

```sh
npm ci --ignore-scripts
npm test
npm run build
npm run pack:candidate
```

[Contributor guide](CONTRIBUTING.md) · [Release preparation](docs/RELEASING.md) · [History index](docs/history/index.md).
Development outputs belong in ignored `build/`; published archives stay frozen.

## Licence

[PolyForm Noncommercial 1.0.0](LICENSE). Preserve bundled third-party notices and attribution.
