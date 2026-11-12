# XOCE Pebble UI

Small native drawing helpers and a phone settings stylesheet shared by XOCE Pebble apps.

## Download

[Version 0.1.1](https://github.com/xefensor/pebble-lib-ui/releases/tag/v0.1.1) is the current published library.

[package](https://github.com/xefensor/pebble-lib-ui/releases/download/v0.1.1/pebble-xoce-ui-0.1.1.tgz),
[SDK archive](https://github.com/xefensor/pebble-lib-ui/releases/download/v0.1.1/dist.zip), and
[checksums](https://github.com/xefensor/pebble-lib-ui/releases/download/v0.1.1/SHA256SUMS.txt).

Package name: `pebble-xoce-ui`. This library is bundled by a host app; it is not installed on
the watch separately.

## Compatibility

The development SDK targets are **Emery / Time 2, Flint / Pebble 2 Duo and Gabbro / Round 2**, using **Pebble SDK 4.44**.

The development candidate adds native PebbleOS Header, MenuCellTitle,
MenuCellSubtitle and Footer roles. `xoce_ui_native_font_key()` selects the exact
system bitmap font for Small, Medium or Large, with the original firmware table
and the completed Large table from firmware 4.44. Hosts supply the firmware
capability and effective size. See [pinned role data and sources](docs/native-type.json).
`xoce_ui_effective_text_size()` maps Emery's OS preference or a matching manual
override. `xoce_ui_native_measure()` and `xoce_ui_native_draw()` use the same font;
`xoce_ui_native_header_layout()` reserves the measured complete clock before
allocating the left label. The library reads no settings and owns no lifecycle.
Watchfaces keep their own fonts and composition.

## Setup and use

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

Content-only icon entrypoints let watchfaces avoid linking action-control helpers.
Timer and Dashboard development candidates use native controls for measured rows
and window navigation. The package owns neither input nor
window navigation.

## Version 0.1.1

- First GitHub release of the shared native drawing helpers and phone settings stylesheet.
- Includes readable type roles, measured glyph centering, canonical icons, full-height action rails, image fitting and stock Pebble action-bar feedback.
- Timer retains its rigid 120 ms selection; Dashboard retains immediate selection. Apps keep ownership of input, state, timers and storage.
- Built with Pebble SDK 4.33.1 for Emery. Validated with portable tests, package checks and consuming-app emulator captures; physical-device and fresh browser testing remain unverified.

## Development

Use Node 22, Python 3.12 and a C compiler. SDK builds additionally need Pebble SDK 4.44 on
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

## Version 0.1.2

Builds for Emery and Gabbro with SDK 4.44. Adds an opt-in round-safe rectangular viewport helper; hosts retain layer, event, input and resource ownership. Existing type profiles and drawing interfaces are unchanged.

This is local release preparation; published downloads above remain frozen.


## Round geometry (0.1.3 development candidate)

`xoce_ui_circle_band(surface, box, inset)` intersects a text/artwork rectangle
with the safe chord across its entire height. Pass the physical display bounds
as `surface`, even during Peek. Measure and draw with the same returned width.
`xoce_ui_round_header_layout` places the complete clock above a contextual title.
Apps still own windows, menus, input, paging and timers. The legacy inscribed
viewport remains available for compatibility; current Gabbro consumers use bands.

The Gabbro action bar follows PebbleOS v4.44.4: 40px width, a circle of diameter
`display_height * 19 / 9`, icon centers at y=89/130/171 and x=239 on 260×260,
and 4px press displacement. Rectangular geometry and feedback retain their
existing values. Native SDK comparisons are separate from portable geometry tests.
