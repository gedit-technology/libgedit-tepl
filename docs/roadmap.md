libgedit-tepl roadmap
=====================

This page contains the plans for major code changes we hope to get done in the
future.

A complete overhaul of the file loading and saving
--------------------------------------------------

- Status: **in progress**

Description:
- New and hopefully simpler implementation, the API will still be mostly the
  same.
- It is expected to have much improved performances (faster loading and
  saving) and a smarter auto-detection of the character encoding.
- Implement both the backend and the frontend, incrementally. For the frontend
  (the high-level API), handle all errors by showing TeplInfoBar's etc.
- Use the [libicu](http://site.icu-project.org/) for character encoding
  _conversion_, not iconv.
- Use the libicu for character encoding _auto-detection_, not
  [uchardet](https://www.freedesktop.org/wiki/Software/uchardet/).
- Compared to the GtkSourceView file loading and saving:
	- Drop gzip compression support and progress callbacks.
	- Add more important features: preventing loading files that are not
	  well supported by GtkTextView (very big file sizes, or very long
	  lines). Ideally proposing solutions to load the files, for example to
	  split very long lines (and then showing a warning when saving the
	  file).

Already done (Tepl 4 -> Tepl 5):
- TeplFile: rework it, keep what is well implemented, and replace the code that
  does sync I/O with async I/O.
- TeplFileLoader and TeplFileSaver: remove the old implementation, re-implement
  the classes in a basic way (UTF-8 only at first).
- TeplEncoding: remove the class, because it was implemented with iconv and
  uchardet in mind.

File browser widget
-------------------

- Status: **todo**

To be integrated in a side panel.

Search and replace UI
---------------------

- Status: **todo**

File printing UI
----------------

- Status: **todo**

High-level spell-checking API
-----------------------------

- Status: **todo**

Integrate [gspell](https://gitlab.gnome.org/GNOME/gspell/) with the
libgedit-tepl framework.
