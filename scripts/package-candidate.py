#!/usr/bin/env python3
"""Pack checked SDK + current sources, without publishing or writing root dist."""
from pathlib import Path
import hashlib
import json
import shutil
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]
sdk = root / 'build/candidate/dist.zip'
record = json.loads((sdk.parent / 'build.json').read_text())
active = {'package.json', 'wscript'}
for directory in ('src', 'include'):
    active.update(str(path.relative_to(root)) for path in (root / directory).rglob('*') if path.is_file())
if active != set(record['inputs']):
    raise SystemExit('Stale SDK candidate: source files added or removed; run npm run build')
for name, checksum in record['inputs'].items():
    if hashlib.sha256((root / name).read_bytes()).hexdigest() != checksum:
        raise SystemExit('Stale SDK candidate: run npm run build after changing ' + name)
if hashlib.sha256(sdk.read_bytes()).hexdigest() != record['sdkSha256']:
    raise SystemExit('Modified SDK candidate: run npm run build')
package = json.loads((root / 'package.json').read_text())
with tempfile.TemporaryDirectory(prefix='xoce-ui-package-') as directory:
    staging = Path(directory)
    for name in ['package.json'] + package['files']:
        source = sdk if name == 'dist.zip' else root / name
        target = staging / name
        if source.is_dir():
            shutil.copytree(source, target)
        elif source.is_file():
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(source, target)
    result = subprocess.run(['npm', 'pack', '--ignore-scripts', '--pack-destination',
                             str(sdk.parent), '--json'], cwd=staging,
                            capture_output=True, text=True, check=True)
    archive = sdk.parent / json.loads(result.stdout)[0]['filename']
    print(archive)
    print('sha256 ' + hashlib.sha256(archive.read_bytes()).hexdigest())
