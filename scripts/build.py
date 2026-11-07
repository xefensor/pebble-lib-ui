#!/usr/bin/env python3
"""Build the local SDK library; all generated files stay under build/."""
from pathlib import Path
import hashlib
import json
import subprocess

root = Path(__file__).resolve().parents[1]
subprocess.run(['pebble', 'build'], cwd=root, check=True)
archive = root / 'build/candidate/dist.zip'
inputs = [root / 'package.json', root / 'wscript']
for directory in ('src', 'include'):
    inputs.extend(path for path in (root / directory).rglob('*') if path.is_file())
fingerprint = {str(path.relative_to(root)): hashlib.sha256(path.read_bytes()).hexdigest()
               for path in sorted(inputs)}
(archive.parent / 'build.json').write_text(json.dumps({
    'inputs': fingerprint,
    'sdkSha256': hashlib.sha256(archive.read_bytes()).hexdigest()
}, indent=2) + '\n')
print(archive)
