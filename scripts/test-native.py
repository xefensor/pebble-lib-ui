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
    subprocess.run([os.environ.get('CC','cc'), '-std=c99', '-Wall', '-Wextra', '-Werror',
                    '-DPBL_ROUND', '-I',str(root/'tests/fake-sdk'),'-I',str(root/'include'),
                    str(root/'src/c/layout.c'),str(root/'tests/layout.c'),'-o',str(binary)],check=True)
    subprocess.run([str(binary)],check=True)
    binary = Path(scratch) / 'native-type'
    subprocess.run([os.environ.get('CC', 'cc'), '-std=c99', '-Wall', '-Wextra',
                    '-Werror', '-pedantic', '-I', str(root / 'tests/fake-sdk'),
                    '-I', str(root / 'include'), str(root / 'src/c/native_type.c'),
                    str(root / 'tests/native-type.c'), str(root / 'src/c/layout.c'), '-o', str(binary)], check=True)
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

# Flint exposes SDK Small/Medium/Large, rather than the large-display offset.
with tempfile.TemporaryDirectory(prefix="xoce-flint-profiles-") as scratch:
    source=Path(scratch)/"flint.c"
    source.write_text('#include "native-type.h"\n#include <assert.h>\n#include <stdio.h>\nint main(void){for(unsigned size=0;size<3;++size){assert(xoce_ui_effective_text_size(0,size)==size);for(unsigned role=0;role<XOCE_UI_TYPE_ROLE_COUNT;++role)puts(xoce_ui_native_font_key(size,(XoceUiTypeRole)role,true));}}\n')
    binary=Path(scratch)/"flint"
    subprocess.run([os.environ.get("CC","cc"),"-std=c99","-Wall","-Wextra","-Werror","-DPBL_PLATFORM_FLINT","-I",str(root/"include"),str(source),"-o",str(binary)],check=True)
    actual=subprocess.check_output([str(binary)],text=True).splitlines()
    assert actual==[row[role] for row in expected["flint"] for role in expected["roles"]]
    print("Flint native roles and OS-size mapping: PASS")
