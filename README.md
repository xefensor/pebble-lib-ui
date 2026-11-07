# XOCE Pebble UI

A small shared implementation of equivalent visual elements in Timer, Dashboard,
Dynamic and Open Activity. Package: `pebble-xoce-ui`. This local 0.1.0 candidate
has not been published.

The package supplies readable font-role measurements, glyph-ink centering,
header geometry, canonical vector icons, action-bar drawing, aspect-preserving
image fitting, physical-button feedback and common phone-settings CSS.
It uses the existing Pebble C SDK; it is not a screen/layout framework.

Apps own their windows, callbacks, timers, font loading, authoritative state,
alarms, persistence, activity/navigation logic and settings controllers. Drawing
helpers receive the host's fonts or text callback and own no allocations,
subscriptions or background work. Icon sizes describe visible geometry; large
Timer illustrations and watchface compositions remain app-specific.

Ordinary app roles are header 26, main 24 and secondary 22, with 8px horizontal
padding and a 32px header. Critical text should wrap or use a dedicated layout
rather than shrink to unreadable sizes. The full-height Emery rail is 34px wide;
its icon centers are 54/114/174 at 228px height. Button feedback is 5px with a 144ms
cubic release. Timer's rigid 120ms menu selection and Dashboard's immediate
selection are intentionally different interaction roles.

```c
#include <pebble-xoce-ui/pebble-xoce-ui.h>
```

```js
var commonStyle = require('pebble-xoce-ui/settings-style');
```

Use Node 22+, Python 3, a C compiler for portable tests and Pebble SDK 4.33.1 for
the SDK build. Source the workspace's recovery-validation/env.sh when using its
local toolchain. Run `npm test`, `npm run build`, then `npm run pack:candidate`.
The SDK archive and npm archive are generated under build/candidate/. Consumers
pin the tarball in vendor/ with a file dependency, lockfile and checksum. Updating
this source does not change an installed app: upgrade and validate each consumer.

No phone or watch installation is needed for this library. No registry, remote
service, new bundler or code generator is required. Common phone CSS is included
at build time; Clay hosts receive it through their supported userData adapter.

Source license and attribution follow the other XOCE Pebble repositories.

Content-only icon entrypoints let watchfaces reuse the same drawings without
linking the action-control family. This keeps Dynamic within SDK 4.33.1’s
64 KiB loaded-image limit. The SDK version is unchanged.
