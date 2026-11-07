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
