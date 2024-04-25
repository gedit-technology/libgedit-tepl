subdir('xml')

gtkdoc_module_name = '@0@-@1@'.format(meson.project_name(), TEPL_API_VERSION)

FS.copyfile(
  '@0@-sections.txt'.format(meson.project_name()),
  '@0@-sections.txt'.format(gtkdoc_module_name)
)

html_dir = get_option('prefix') / GNOME.gtkdoc_html_dir(gtkdoc_module_name)

glib_docpath = dependency('glib-2.0').get_variable(pkgconfig: 'prefix') / 'share/gtk-doc/html/glib'
gobject_docpath = dependency('gobject-2.0').get_variable(pkgconfig: 'prefix') / 'share/gtk-doc/html/gobject'
gio_docpath = dependency('gio-2.0').get_variable(pkgconfig: 'prefix') / 'share/gtk-doc/html/gio'
gtk_docpath = dependency('gtk+-3.0').get_variable(pkgconfig: 'prefix') / 'share/gtk-doc/html/gtk3'
gsv_docpath = dependency('libgedit-gtksourceview-300').get_variable(pkgconfig: 'prefix') / 'share/gtk-doc/html/libgedit-gtksourceview-300'
amtk_docpath = dependency('libgedit-amtk-5').get_variable(pkgconfig: 'prefix') / 'share/gtk-doc/html/libgedit-amtk-5'
gfls_docpath = dependency('libgedit-gfls-1').get_variable(pkgconfig: 'prefix') / 'share/gtk-doc/html/libgedit-gfls-1'

GNOME.gtkdoc(
  gtkdoc_module_name,
  main_xml: '@0@-docs.xml'.format(meson.project_name()),
  src_dir: include_directories('../../tepl/'),
  dependencies: TEPL_LIB_DEP,
  scan_args: ['--rebuild-types'],
  gobject_typesfile: '@0@.types'.format(gtkdoc_module_name),
  fixxref_args: [
    '--html-dir=@0@'.format(html_dir),
    '--extra-dir=@0@'.format(glib_docpath),
    '--extra-dir=@0@'.format(gobject_docpath),
    '--extra-dir=@0@'.format(gio_docpath),
    '--extra-dir=@0@'.format(gtk_docpath),
    '--extra-dir=@0@'.format(gsv_docpath),
    '--extra-dir=@0@'.format(amtk_docpath),
    '--extra-dir=@0@'.format(gfls_docpath)
  ],
  content_files: [
    'api-breaks.xml',
    'intro.xml',
  ],
  ignore_headers: TEPL_PRIVATE_HEADERS,
  install: true
)
