#!/usr/bin/env python3
"""Audit each consumer's own UI pin; source equality to this checkout is separate."""
from pathlib import Path, PurePosixPath
import argparse
import base64
import hashlib
import io
import json
import sys
import tarfile

ROOT = Path(__file__).resolve().parents[1]
NAME = 'pebble-xoce-ui'
CONSUMERS = ('pebble-app-timer', 'pebble-app-dashboard',
             'pebble-watchface-dynamic', 'halftone-watchface',
             'pebble-app-open-activity')


def require(condition, message):
    if not condition:
        raise ValueError(message)


def is_source(path):
    return path.startswith(('src/', 'include/')) or (
        '/' not in path and path.endswith(('.js', '.h', '.c')))


def sources(files):
    return {path: data for path, data in files.items() if is_source(path)}


def checkout_sources(root):
    paths = list(root.glob('*'))
    for directory in ('src', 'include'):
        paths.extend((root / directory).rglob('*'))
    return {str(path.relative_to(root)): path.read_bytes() for path in paths
            if path.is_file() and is_source(str(path.relative_to(root)))}


def archive_files(raw):
    files = {}
    with tarfile.open(fileobj=io.BytesIO(raw), mode='r:gz') as archive:
        for member in archive.getmembers():
            if member.isdir():
                continue
            path = PurePosixPath(member.name)
            require(member.isfile() and path.parts and path.parts[0] == 'package' and
                    '..' not in path.parts and len(path.parts) > 1,
                    'Invalid package archive member: ' + member.name)
            name = str(PurePosixPath(*path.parts[1:]))
            require(name not in files, 'Duplicate package member: ' + name)
            files[name] = archive.extractfile(member).read()
    return files


def check_consumer(app, root):
    pin = json.loads((app / 'vendor/UI.json').read_text())
    require(pin['package'] == NAME, 'Wrong UI package name')
    archive_name = pin['archive']
    require(isinstance(archive_name, str) and Path(archive_name).name == archive_name,
            'UI archive must be a filename inside vendor/')
    raw = (app / 'vendor' / archive_name).read_bytes()
    digest = hashlib.sha256(raw).hexdigest()
    integrity = 'sha512-' + base64.b64encode(hashlib.sha512(raw).digest()).decode()
    require(pin['sha256'] == digest and pin['integrity'] == integrity,
            'UI archive differs from its provenance checksums')

    version = pin['version']
    reference = 'file:vendor/' + archive_name
    package = json.loads((app / 'package.json').read_text())
    lock = json.loads((app / 'package-lock.json').read_text())
    require(package['dependencies'][NAME] == reference and
            lock['packages']['']['dependencies'][NAME] == reference,
            'UI dependency differs from its pinned archive')
    entry = lock['packages']['node_modules/' + NAME]
    require(entry['version'] == version and entry['resolved'] == reference and
            entry['integrity'] == integrity, 'UI lockfile identity differs from its pin')

    files = archive_files(raw)
    manifest = json.loads(files['package.json'])
    require(manifest['name'] == NAME and manifest['version'] == version,
            'UI archive package identity differs from its pin')
    require('src/js/settings-style.js' in files and 'dist.zip' in files and
            any(path.startswith('include/') for path in files) and
            any(path.startswith('src/c/') for path in files),
            'UI archive is missing its stylesheet, native sources or SDK')
    installed = app / 'node_modules' / NAME
    installed_manifest = json.loads((installed / 'package.json').read_text())
    require(installed_manifest['name'] == NAME and installed_manifest['version'] == version,
            'Installed UI package identity differs from its pin; run npm ci')
    require(checkout_sources(installed) == sources(files),
            'Installed UI file differs from its archive: source tree differs')
    for path, data in files.items():
        if path.startswith(('src/', 'include/')) or path.endswith(('.js', '.h', '.c')) or path == 'dist.zip':
            require((installed / path).read_bytes() == data,
                    'Installed UI file differs from its archive: ' + path)

    checkout_version = json.loads((root / 'package.json').read_text())['version']
    same_sources = sources(files) == checkout_sources(root)
    return {
        'app': app.name, 'version': version, 'archiveSha256': digest,
        'stylesheetSha256': hashlib.sha256(files['src/js/settings-style.js']).hexdigest(),
        'declaredSourceCommit': pin.get('source_commit'),
        'declaredStatus': pin.get('status'),
        'matchesCheckoutVersion': version == checkout_version,
        'matchesCheckoutSources': same_sources,
        'matchesCheckoutVersionAndSources': version == checkout_version and same_sources,
    }


def audit(root, apps):
    reports = []
    font_hashes = {}
    for app in apps:
        try:
            reports.append(check_consumer(app, root))
        except (OSError, ValueError, KeyError, tarfile.TarError) as error:
            raise ValueError(str(app) + ': ' + str(error)) from error
        for font in ('RobotoCondensed-Bold.ttf', 'Fredoka-Bold-Clock.ttf'):
            path = app / 'resources/fonts' / font
            if path.exists():
                font_hashes.setdefault(font, {})[app.name] = hashlib.sha256(path.read_bytes()).hexdigest()
    return {
        'result': 'PASS',
        'scope': 'Each consumer pin, archive, lockfile and installed sources/SDK; no build or runtime validation',
        'checkoutVersion': json.loads((root / 'package.json').read_text())['version'],
        'consumers': reports,
        'sameArchiveAcrossConsumers': len({r['archiveSha256'] for r in reports}) == 1,
        'fonts': font_hashes,
        'sameBytesPerFont': {font: len(set(copies.values())) == 1 for font, copies in font_hashes.items()},
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('apps', type=Path, nargs='*', help='App checkout paths; defaults to five XOCE sibling apps')
    parser.add_argument('--output', type=Path, default=ROOT / 'build/qa/consumer-pins.json')
    args = parser.parse_args()
    apps = args.apps or [ROOT.parent / name for name in CONSUMERS]
    try:
        report = audit(ROOT, apps)
    except ValueError as error:
        print(error, file=sys.stderr)
        return 1
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print('Consumer UI pins/installed bytes PASS:', len(report['consumers']))
    print('Comparison checkout:', report['checkoutVersion'], '(see per-consumer source/version fields)')
    print('No app build, runtime or visual validation was performed.')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
