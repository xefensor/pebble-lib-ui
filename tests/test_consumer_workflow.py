"""Check independent consumer pins without claiming app/runtime validation."""
from pathlib import Path
import base64
import hashlib
import importlib.util
import io
import json
import tarfile
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location('consumer_pins', ROOT / 'scripts/check-consumers.py')
consumer_pins = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(consumer_pins)


class ConsumerWorkflow(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.workspace = Path(self.temp.name)
        self.root = self.workspace / 'pebble-lib-ui'
        self.write_json(self.root / 'package.json', {'name': 'pebble-xoce-ui', 'version': '0.1.1'})
        self.original_sources = {
            'settings-style.js': b'original stylesheet export',
            'pebble-xoce-ui.h': b'original header export',
            'compat.c': b'original C export',
            'src/js/settings-style.js': b'original stylesheet',
            'src/c/layout.c': b'original native source',
            'include/pebble-xoce-ui.h': b'original header',
        }
        for name, data in self.original_sources.items():
            self.write(self.root / name, data)

    def write(self, path, data):
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(data)

    def write_json(self, path, data):
        self.write(path, json.dumps(data).encode())

    def consumer(self, name='app', version='0.1.3', changed=True, manifest_version=None):
        app = self.workspace / name
        files = dict(self.original_sources)
        if changed:
            files['src/js/settings-style.js'] = b'newer independently pinned stylesheet'
            files['include/new.h'] = b'newer independently pinned header'
        files['package.json'] = json.dumps({
            'name': 'pebble-xoce-ui', 'version': manifest_version or version,
        }).encode()
        files['dist.zip'] = b'pinned SDK bytes'
        buffer = io.BytesIO()
        with tarfile.open(fileobj=buffer, mode='w:gz') as archive:
            for path, data in files.items():
                member = tarfile.TarInfo('package/' + path)
                member.size = len(data)
                archive.addfile(member, io.BytesIO(data))
        raw = buffer.getvalue()
        archive_name = 'pebble-xoce-ui-' + version + '.tgz'
        integrity = 'sha512-' + base64.b64encode(hashlib.sha512(raw).digest()).decode()
        self.write(app / 'vendor' / archive_name, raw)
        self.write_json(app / 'vendor/UI.json', {
            'package': 'pebble-xoce-ui', 'version': version, 'archive': archive_name,
            'sha256': hashlib.sha256(raw).hexdigest(), 'integrity': integrity,
            'source_commit': 'fixture-source', 'status': 'fixture snapshot, not a release',
        })
        reference = 'file:vendor/' + archive_name
        self.write_json(app / 'package.json', {'dependencies': {'pebble-xoce-ui': reference}})
        self.write_json(app / 'package-lock.json', {'packages': {
            '': {'dependencies': {'pebble-xoce-ui': reference}},
            'node_modules/pebble-xoce-ui': {
                'version': version, 'resolved': reference, 'integrity': integrity,
            },
        }})
        for path, data in files.items():
            self.write(app / 'node_modules/pebble-xoce-ui' / path, data)
        return app

    def test_newer_pin_passes_without_claiming_checkout_source_equality(self):
        report = consumer_pins.audit(self.root, [self.consumer()])
        self.assertEqual(report['result'], 'PASS')
        self.assertEqual(report['checkoutVersion'], '0.1.1')
        pin = report['consumers'][0]
        self.assertEqual(pin['version'], '0.1.3')
        self.assertFalse(pin['matchesCheckoutVersion'])
        self.assertFalse(pin['matchesCheckoutSources'])
        self.assertFalse(pin['matchesCheckoutVersionAndSources'])
        self.assertEqual(pin['declaredStatus'], 'fixture snapshot, not a release')

    def test_different_consumer_pins_are_valid_and_reported(self):
        old = self.consumer('old-app', '0.1.1', changed=False)
        new = self.consumer('new-app', '0.1.3')
        report = consumer_pins.audit(self.root, [old, new])
        self.assertFalse(report['sameArchiveAcrossConsumers'])
        self.assertTrue(report['consumers'][0]['matchesCheckoutVersionAndSources'])
        self.assertFalse(report['consumers'][1]['matchesCheckoutVersionAndSources'])

    def test_same_sources_with_different_version_do_not_claim_same_checkout(self):
        pin = consumer_pins.check_consumer(self.consumer(changed=False), self.root)
        self.assertTrue(pin['matchesCheckoutSources'])
        self.assertFalse(pin['matchesCheckoutVersionAndSources'])

    def test_changed_archive_is_rejected_before_install_comparison(self):
        app = self.consumer()
        path = app / 'vendor/pebble-xoce-ui-0.1.3.tgz'
        path.write_bytes(path.read_bytes() + b'changed')
        with self.assertRaisesRegex(ValueError, 'provenance checksums'):
            consumer_pins.audit(self.root, [app])

    def test_lockfile_must_match_the_consumer_pin_version(self):
        app = self.consumer()
        path = app / 'package-lock.json'
        lock = json.loads(path.read_text())
        lock['packages']['node_modules/pebble-xoce-ui']['version'] = '0.1.1'
        self.write_json(path, lock)
        with self.assertRaisesRegex(ValueError, 'lockfile identity'):
            consumer_pins.audit(self.root, [app])

    def test_archive_manifest_must_match_the_declared_pin(self):
        app = self.consumer(manifest_version='0.1.1')
        with self.assertRaisesRegex(ValueError, 'archive package identity'):
            consumer_pins.audit(self.root, [app])

    def test_pinned_sources_and_sdk_include_newer_package_files(self):
        for path in ('include/new.h', 'src/js/settings-style.js', 'settings-style.js',
                     'pebble-xoce-ui.h', 'compat.c', 'dist.zip'):
            with self.subTest(path=path):
                app = self.consumer(name=path.replace('/', '-'))
                self.write(app / 'node_modules/pebble-xoce-ui' / path, b'tampered')
                with self.assertRaisesRegex(ValueError, 'Installed UI file differs'):
                    consumer_pins.audit(self.root, [app])

    def test_font_differences_are_informational(self):
        first, second = self.consumer('first'), self.consumer('second')
        for app in (first, second):
            self.write(app / 'resources/fonts/RobotoCondensed-Bold.ttf', app.name.encode())
        report = consumer_pins.audit(self.root, [first, second])
        self.assertEqual(report['result'], 'PASS')
        self.assertFalse(report['sameBytesPerFont']['RobotoCondensed-Bold.ttf'])

    def test_extra_installed_source_cannot_pass_as_the_pinned_source_tree(self):
        app = self.consumer()
        self.write(app / 'node_modules/pebble-xoce-ui/src/js/stale.js', b'old installed source')
        with self.assertRaisesRegex(ValueError, 'source tree differs'):
            consumer_pins.audit(self.root, [app])

    def test_stale_root_exports_prevent_checkout_source_equivalence(self):
        for path in ('settings-style.js', 'pebble-xoce-ui.h', 'compat.c'):
            with self.subTest(path=path):
                app = self.consumer(name=path, version='0.1.1', changed=False)
                self.write(self.root / path, b'changed checkout root export')
                pin = consumer_pins.check_consumer(app, self.root)
                self.assertTrue(pin['matchesCheckoutVersion'])
                self.assertFalse(pin['matchesCheckoutSources'])
                self.assertFalse(pin['matchesCheckoutVersionAndSources'])
                self.write(self.root / path, self.original_sources[path])

    def test_extra_installed_root_export_cannot_pass_as_pinned_sources(self):
        for path in ('stale.js', 'stale.h', 'stale.c'):
            with self.subTest(path=path):
                app = self.consumer(name=path)
                self.write(app / 'node_modules/pebble-xoce-ui' / path, b'stale root export')
                with self.assertRaisesRegex(ValueError, 'source tree differs'):
                    consumer_pins.audit(self.root, [app])


if __name__ == '__main__':
    unittest.main()
