#!/usr/bin/env python3
"""Check this workspace's four explicit UI pins; no app installation or edits."""
from pathlib import Path
import base64
import hashlib
import io
import json
import tarfile

root = Path(__file__).resolve().parents[1]
version = json.loads((root / 'package.json').read_text())['version']
name = 'pebble-xoce-ui'
reports = []
font_hashes = {}
for directory in ('pebble-app-timer', 'pebble-app-dashboard',
                  'pebble-watchface-dynamic', 'pebble-app-open-activity'):
    app = root.parent / directory
    pin = json.loads((app / 'vendor/UI.json').read_text())
    raw = (app / 'vendor' / pin['archive']).read_bytes()
    digest = hashlib.sha256(raw).hexdigest()
    integrity = 'sha512-' + base64.b64encode(hashlib.sha512(raw).digest()).decode()
    reference = 'file:vendor/' + pin['archive']
    package = json.loads((app / 'package.json').read_text())
    lock = json.loads((app / 'package-lock.json').read_text())
    assert pin['package'] == name and pin['version'] == version, directory + ': UI version differs'
    assert pin['sha256'] == digest and pin['integrity'] == integrity, directory + ': archive changed'
    assert package['dependencies'][name] == reference
    assert lock['packages']['']['dependencies'][name] == reference
    entry = lock['packages']['node_modules/' + name]
    assert entry['version'] == version and entry['resolved'] == reference and entry['integrity'] == integrity
    with tarfile.open(fileobj=io.BytesIO(raw), mode='r:gz') as archive:
        files = {member.name.removeprefix('package/'): archive.extractfile(member).read()
                 for member in archive.getmembers() if member.isfile()}
    installed = app / 'node_modules' / name
    for font in ('RobotoCondensed-Bold.ttf', 'Fredoka-Bold-Clock.ttf'):
        source = app / 'resources/fonts' / font
        if source.exists():
            font_hashes.setdefault(font, {})[directory] = hashlib.sha256(source.read_bytes()).hexdigest()
    for path in ('src', 'include'):
        for source in (root / path).rglob('*'):
            if not source.is_file():
                continue
            relative = str(source.relative_to(root))
            assert files[relative] == source.read_bytes(), directory + ': stale source ' + relative
            assert (installed / relative).read_bytes() == files[relative], directory + ': installed source differs'
    assert (installed / 'dist.zip').read_bytes() == files['dist.zip'], directory + ': installed SDK differs'
    reports.append({'app': directory, 'version': version, 'archiveSha256': digest,
                    'stylesheetSha256': hashlib.sha256(files['src/js/settings-style.js']).hexdigest()})
assert len({report['archiveSha256'] for report in reports}) == 1, 'Consumers use different UI packages'
for font, copies in font_hashes.items():
    assert len(set(copies.values())) == 1, 'Shared font source differs: ' + font
output = root / 'build/qa/consumer-pins.json'
output.parent.mkdir(parents=True, exist_ok=True)
output.write_text(json.dumps({'result': 'PASS', 'consumers': reports, 'fonts': font_hashes}, indent=2) + '\n')
print('All four consumers use the same checked UI sources, SDK and stylesheet:', version)
