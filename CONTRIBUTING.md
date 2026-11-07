# Development and visual checks

Run `npm test` and `npm run build` for library changes. `npm run pack:candidate`
creates the local archive under build/candidate/. Preserve the independent
release pins of existing apps; upgrade all four UI consumers together when
changing shared appearance.

In the XOCE workspace, run `python3 scripts/check-consumers.py` to verify that
Timer, Dashboard, Dynamic and Open Activity use the same archive, matching
lockfile integrity, installed SDK, source helpers and stylesheet. This workspace
check is optional for standalone library development and writes only build/qa/.

App repositories retain their own portable tests, packaged workflow checks and
native emulator checks. New captures belong in build/qa/shared-ui/ in each app.
Use the actual app build and synthetic fixtures, inspect whole compositions and
compare equivalent roles side by side. Historical captures must stay frozen.
Keep browser, native emulator, simulated phone and physical-device evidence
distinct. A source check does not verify browser layout or real-watch battery.

Changes are local until explicitly published. A package update is compiled into
each app; it is not a separately installed service or an automatic global update.
