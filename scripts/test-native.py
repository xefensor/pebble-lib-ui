#!/usr/bin/env python3
"""Portable geometry/feedback checks. SDK and real rendering remain separate."""
from pathlib import Path
import os
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]
with tempfile.TemporaryDirectory(prefix='xoce-ui-test-') as scratch:
    binary = Path(scratch) / 'layout'
    subprocess.run([os.environ.get('CC', 'cc'), '-std=c99', '-Wall', '-Wextra',
                    '-Werror', '-pedantic', '-I', str(root / 'tests/fake-sdk'),
                    '-I', str(root / 'include'), str(root / 'src/c/layout.c'),
                    str(root / 'tests/layout.c'), '-o', str(binary)], check=True)
    subprocess.run([str(binary)], check=True)
    binary = Path(scratch) / 'native-type'
    subprocess.run([os.environ.get('CC', 'cc'), '-std=c99', '-Wall', '-Wextra',
                    '-Werror', '-pedantic', '-I', str(root / 'tests/fake-sdk'),
                    '-I', str(root / 'include'), str(root / 'src/c/native_type.c'),
                    str(root / 'tests/native-type.c'), '-o', str(binary)], check=True)
    subprocess.run([str(binary)], check=True)

# Compare every public role against independently pinned upstream tables.
import json
expected = json.loads((root / 'docs/native-type.json').read_text())
with tempfile.TemporaryDirectory(prefix='xoce-native-profiles-') as scratch:
    source = Path(scratch) / 'profiles.c'
    source.write_text('#include "native-type.h"\n#include <stdio.h>\nint main(void) {\n  for(unsigned modern=0;modern<2;++modern)\n    for(unsigned size=0;size<3;++size)\n      for(unsigned role=0;role<XOCE_UI_TYPE_ROLE_COUNT;++role)\n        puts(xoce_ui_native_font_key(size,(XoceUiTypeRole)role,modern));\n}\n')
    binary = Path(scratch) / 'profiles'
    subprocess.run([os.environ.get('CC', 'cc'), '-std=c99', '-Wall', '-Wextra', '-Werror',
                    '-I', str(root / 'include'), str(source), '-o', str(binary)], check=True)
    actual = subprocess.check_output([str(binary)], text=True).splitlines()
    wanted = [row[role] for family in ('baseline', 'modern') for row in expected[family] for role in expected['roles']]
    assert actual == wanted
    print('All native font keys match pinned PebbleOS semantic roles: PASS')
