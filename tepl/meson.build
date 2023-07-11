tepl_public_headers = [
  'tepl.h',
  'tepl-abstract-factory.h',
  'tepl-application.h',
  'tepl-application-window.h',
  'tepl-buffer.h',
  'tepl-encoding.h',
  'tepl-encoding-iconv.h',
  'tepl-file.h',
  'tepl-file-chooser.h',
  'tepl-file-loader.h',
  'tepl-file-saver.h',
  'tepl-fold-region.h',
  'tepl-goto-line-bar.h',
  'tepl-gutter-renderer-folds.h',
  'tepl-info-bar.h',
  'tepl-init.h',
  'tepl-io-error-info-bars.h',
  'tepl-iter.h',
  'tepl-language-chooser.h',
  'tepl-language-chooser-dialog.h',
  'tepl-language-chooser-widget.h',
  'tepl-line-column-indicator.h',
  'tepl-macros.h',
  'tepl-menu-shell.h',
  'tepl-metadata.h',
  'tepl-metadata-manager.h',
  'tepl-notebook.h',
  'tepl-overwrite-indicator.h',
  'tepl-panel1.h',
  'tepl-pango.h',
  'tepl-prefs.h',
  'tepl-prefs-dialog.h',
  'tepl-progress-info-bar.h',
  'tepl-settings.h',
  'tepl-signal-group.h',
  'tepl-space-drawer-prefs.h',
  'tepl-stack.h',
  'tepl-stack-item.h',
  'tepl-stack-switcher-menu.h',
  'tepl-statusbar.h',
  'tepl-style-scheme-chooser-widget.h',
  'tepl-tab.h',
  'tepl-tab-group.h',
  'tepl-tab-label.h',
  'tepl-tab-loading.h',
  'tepl-tab-saving.h',
  'tepl-utils.h',
  'tepl-view.h',
]

tepl_public_c_files = [
  'tepl-abstract-factory.c',
  'tepl-application.c',
  'tepl-application-window.c',
  'tepl-buffer.c',
  'tepl-encoding.c',
  'tepl-encoding-iconv.c',
  'tepl-file.c',
  'tepl-file-chooser.c',
  'tepl-file-loader.c',
  'tepl-file-saver.c',
  'tepl-fold-region.c',
  'tepl-goto-line-bar.c',
  'tepl-gutter-renderer-folds.c',
  'tepl-info-bar.c',
  'tepl-init.c',
  'tepl-io-error-info-bars.c',
  'tepl-iter.c',
  'tepl-language-chooser.c',
  'tepl-language-chooser-dialog.c',
  'tepl-language-chooser-widget.c',
  'tepl-line-column-indicator.c',
  'tepl-menu-shell.c',
  'tepl-metadata.c',
  'tepl-metadata-manager.c',
  'tepl-notebook.c',
  'tepl-overwrite-indicator.c',
  'tepl-panel1.c',
  'tepl-pango.c',
  'tepl-prefs.c',
  'tepl-prefs-dialog.c',
  'tepl-progress-info-bar.c',
  'tepl-settings.c',
  'tepl-signal-group.c',
  'tepl-space-drawer-prefs.c',
  'tepl-stack.c',
  'tepl-stack-item.c',
  'tepl-stack-switcher-menu.c',
  'tepl-statusbar.c',
  'tepl-style-scheme-chooser-widget.c',
  'tepl-tab.c',
  'tepl-tab-group.c',
  'tepl-tab-label.c',
  'tepl-tab-loading.c',
  'tepl-tab-saving.c',
  'tepl-utils.c',
  'tepl-view.c',
]

TEPL_PRIVATE_HEADERS = [
  'tepl-close-confirm-dialog-single.h',
  'tepl-encoding-iconv-private.h',
  'tepl-icu.h',
  'tepl-metadata-attic.h',
  'tepl-metadata-parser.h',
  'tepl-window-actions-edit.h',
  'tepl-window-actions-file.h',
  'tepl-window-actions-search.h'
]

tepl_private_c_files = [
  'tepl-close-confirm-dialog-single.c',
  'tepl-icu.c',
  'tepl-metadata-attic.c',
  'tepl-metadata-parser.c',
  'tepl-window-actions-edit.c',
  'tepl-window-actions-file.c',
  'tepl-window-actions-search.c'
]

headers_install_dir = get_option('includedir') / 'tepl-@0@/tepl/'.format(TEPL_API_VERSION)
install_headers(
  tepl_public_headers,
  install_dir: headers_install_dir
)

tepl_enum_types = GNOME.mkenums_simple(
  'tepl-enum-types',
  decorator: '_TEPL_EXTERN',
  header_prefix: '#include <tepl/tepl-macros.h>',
  sources: tepl_public_headers,
  install_header: true,
  install_dir: headers_install_dir
)

tepl_static_lib_c_args = [ '-DTEPL_COMPILATION' ]
if meson.get_compiler('c').get_argument_syntax() == 'msvc'
  tepl_static_lib_c_args += '-D_TEPL_EXTERN=__declspec(dllexport) extern'
endif

tepl_static_lib = static_library(
  'tepl-static',
  [tepl_public_c_files,
   tepl_private_c_files,
   tepl_enum_types],
  pic: true, # tepl_static_lib is linked in a shared library.
  include_directories: ROOT_INCLUDE_DIR,
  dependencies: TEPL_DEPS,
  c_args: tepl_static_lib_c_args
)

# For unit tests, to be able to test private functions.
TEPL_STATIC_DEP = declare_dependency(
  include_directories: ROOT_INCLUDE_DIR,
  link_with: tepl_static_lib,
  sources: tepl_enum_types[1],
  dependencies: TEPL_DEPS
)

symbol_map = 'symbol.map'
tepl_lib_link_args = c_compiler.get_supported_link_arguments([
  '-Wl,--version-script,@0@/@1@'.format(meson.current_source_dir(), symbol_map),
])

tepl_lib = library(
  'tepl-@0@'.format(TEPL_API_VERSION),
  dependencies: TEPL_DEPS,
  link_args: tepl_lib_link_args,
  link_depends: symbol_map,
  # link_whole is not supported with MSVC, so we use extract_all_objects().
  objects: tepl_static_lib.extract_all_objects(recursive: false),
  soversion: 3,
  install: true
)

TEPL_LIB_DEP = declare_dependency(
  include_directories: ROOT_INCLUDE_DIR,
  link_with: tepl_lib,
  sources: tepl_enum_types[1],
  dependencies: TEPL_DEPS
)

PKG_CONFIG.generate(tepl_lib,
  filebase: 'tepl-@0@'.format(TEPL_API_VERSION),
  name: 'Tepl',
  description: 'Text editor product line',
  subdirs: 'tepl-@0@'.format(TEPL_API_VERSION),
  libraries: TEPL_PUBLIC_DEPS,
)

if get_option('gobject_introspection')
  GNOME.generate_gir(
    tepl_lib,
    export_packages: 'tepl-@0@'.format(TEPL_API_VERSION),
    header: 'tepl/tepl.h',
    identifier_prefix: 'Tepl',
    include_directories: ROOT_INCLUDE_DIR,
    includes: ['Gtk-3.0', 'GtkSource-300', 'Amtk-5'],
    install: true,
    namespace: 'Tepl',
    nsversion: TEPL_API_VERSION,
    sources: [
      tepl_public_headers,
      tepl_public_c_files,
      tepl_enum_types
    ],
    # Support for deps being built as subprojects:
    dependencies: TEPL_DEPS,
  )
endif
