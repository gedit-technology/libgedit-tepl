# Convention:
# - Local variables in lower_case.
# - Global variables in UPPER_CASE.
# See: https://github.com/mesonbuild/meson/issues/2607

project(
  'libgedit-tepl', 'c',
  meson_version: '>= 0.64',
  version: '6.10.1',
  default_options: ['warning_level=2']
)

GNOME = import('gnome')
PKG_CONFIG = import('pkgconfig')
I18N = import('i18n')
FS = import('fs')

# API version, used for parallel installability.
TEPL_API_VERSION = '6'

TEPL_PUBLIC_DEPS = [
  dependency('gio-2.0', version: '>= 2.74'),
  dependency('gtk+-3.0', version: '>= 3.22'),
  dependency('libgedit-gtksourceview-300', version: '>= 299.1.0'),
  dependency('libgedit-amtk-5'),
  dependency('libgedit-gfls-1'),
]
tepl_private_deps = [
  dependency('icu-uc'),
  dependency('icu-i18n'),
  dependency('gsettings-desktop-schemas', version: '>= 42'),
  dependency('libhandy-1', version: '>= 1.6'),
]
TEPL_DEPS = [TEPL_PUBLIC_DEPS, tepl_private_deps]

# config.h

config_data = configuration_data()
GETTEXT_PACKAGE_NAME = '@0@-@1@'.format(meson.project_name(), TEPL_API_VERSION)
config_data.set_quoted('GETTEXT_PACKAGE', GETTEXT_PACKAGE_NAME)
config_data.set_quoted('TEPL_LOCALEDIR', get_option('prefix') / get_option('localedir'))

configure_file(
  output: 'config.h',
  configuration: config_data
)

# Misc

ROOT_INCLUDE_DIR = include_directories('.')

add_project_arguments(
  '-DG_LOG_DOMAIN="@0@"'.format(meson.project_name()),
  language: 'c'
)

#####
# CFLAGS
# Some flags are missing when using only the builtin warning_level meson option,
# even at the maximum level.
# The following warning_cflags suppose that warning_level=2.

c_compiler = meson.get_compiler('c')
warning_cflags = []

if c_compiler.get_id() == 'msvc'
  # Use GLib's msvc_recommended_pragmas.h to filter out
  # the warnings we don't really need to worry about,
  # but do make the ones we want to be wary stand out
  warning_cflags += [
    '-FImsvc_recommended_pragmas.h',
  ]
else
  # Try to mimic the AX_COMPILER_FLAGS Autotools macro.
  warning_cflags += [
    '-fno-strict-aliasing',
    '-Wundef',
    '-Wnested-externs',
    '-Wwrite-strings',
    '-Wpointer-arith',
    '-Wmissing-declarations',
    '-Wmissing-prototypes',
    '-Wstrict-prototypes',
    '-Wredundant-decls',
    '-Wno-unused-parameter',
    '-Wno-missing-field-initializers',
    '-Wdeclaration-after-statement',
    '-Wformat=2',
    '-Wold-style-definition',
    '-Wcast-align',
    '-Wformat-nonliteral',
    '-Wformat-security',
    '-Wsign-compare',
    '-Wstrict-aliasing',
    '-Wshadow',
    '-Winline',
    '-Wpacked',
    '-Wmissing-format-attribute',
    '-Wmissing-noreturn',
    '-Winit-self',
    '-Wredundant-decls',
    '-Wmissing-include-dirs',
    '-Wunused-but-set-variable',
    '-Warray-bounds',
    '-Wimplicit-function-declaration',
    '-Wreturn-type',
    '-Wswitch-enum',
    '-Wswitch-default',
    '-Wduplicated-cond',
    '-Wduplicated-branches',
    '-Wlogical-op',
    '-Wrestrict',
    '-Wnull-dereference',
    '-Wjump-misses-init',
    '-Wdouble-promotion'
  ]
endif

supported_warning_cflags = c_compiler.get_supported_arguments(warning_cflags)
add_project_arguments(supported_warning_cflags, language: 'c')
##### end CFLAGS

subdir('po')
subdir('tepl')

if get_option('tests')
  subdir('tests')
endif

if get_option('gtk_doc')
  subdir('docs/reference')
endif

summary('API version', TEPL_API_VERSION)
summary('Prefix', get_option('prefix'))
summary('GObject Introspection', get_option('gobject_introspection'))
summary('API documentation', get_option('gtk_doc'))
summary('Build tests', get_option('tests'))
