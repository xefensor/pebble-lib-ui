# XOCE UI maintainer instructions

Keep this a small drawing and stylesheet package. Apps own windows, input,
timers, state, alarms, storage, font loading and settings controllers. Extract
existing shared presentation; do not introduce a screen framework or event system.

Preserve readable type, full-height action rails, Timer's rigid 120ms menu motion
and Dashboard's immediate selection. Equivalent controls use the same assets,
measurements and feedback. Watchfaces may retain their distinct composition.

Read README.md for the public boundary and development commands. Run npm test
and build a current SDK candidate for native changes. Inspect consuming apps;
portable tests cannot establish visual quality or real-device behavior.
Development outputs belong in build/. Do not publish, push or tag implicitly.
