import os
import shutil
import waflib

top = '.'
out = 'build'


def distclean(ctx):
    if os.path.exists('dist'):
        shutil.rmtree('dist')
    waflib.Scripting.distclean(ctx)


def options(ctx):
    ctx.load('pebble_sdk_lib')

def configure(ctx):
    ctx.load('pebble_sdk_lib')
    ctx.env.BUNDLE_NAME = 'build/candidate/dist.zip'

def build(ctx):
    ctx.load('pebble_sdk_lib')
    ctx.env.BUNDLE_NAME = 'build/candidate/dist.zip'
    ctx.path.make_node('build/candidate').mkdir()
    cached_env = ctx.env
    for platform in ctx.env.TARGET_PLATFORMS:
        ctx.env = ctx.all_envs[platform]
        # GCC 14 link-time optimization removes duplicate static helpers across
        # the host and its libraries. Fat objects retain ordinary archive use.
        ctx.env.append_unique('CFLAGS', ['-flto', '-ffat-lto-objects'])
        ctx.env.append_unique('LINKFLAGS', ['-flto'])
        ctx.set_group(ctx.env.PLATFORM_NAME)
        ctx.pbl_build(source=ctx.path.ant_glob('src/c/**/*.c'),
                      target='{}/{}'.format(ctx.env.BUILD_DIR, ctx.env.PROJECT_INFO['name']),
                      bin_type='lib')
    ctx.env = cached_env
    ctx.set_group('bundle')
    ctx.pbl_bundle(includes=ctx.path.ant_glob('include/**/*.h'),
                   js=ctx.path.ant_glob('src/js/**/*.js'), bin_type='lib')
