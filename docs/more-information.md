libgedit-tepl - more information
================================

Versioning scheme
-----------------

libgedit-tepl follows the even/odd minor version scheme.

For example the `6.3.x` versions are development snapshots (pre-releases), and
the `6.4.x` series is stable.

There are sometimes API or ABI breaks, in which case the library soname is
adapted.

Renames
-------

- The first name was “Gtef” (GTK text editor framework).
- In June 2017 the project has been renamed to “Tepl” to have a more beautiful
  name.
- In April 2024 the project has been renamed to “libgedit-tepl”.

Dependencies
------------

- GTK 3
- [ICU](https://icu.unicode.org/)

From [Gedit Technology](https://gedit-technology.github.io/):
- libgedit-amtk
- libgedit-gtksourceview

Documentation
-------------

See the `gtk_doc` Meson option. A convenient way to read the API documentation
is with the [Devhelp](https://github.com/gdev-technology/devhelp) application.

See also other files in this directory for additional notes.

Tarballs
--------

- [Old Tepl tarballs](https://download.gnome.org/sources/tepl/)
- [Old Gtef tarballs](https://download.gnome.org/sources/gtef/)
