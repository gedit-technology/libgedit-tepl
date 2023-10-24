/* SPDX-FileCopyrightText: 2020 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_IO_ERROR_INFO_BARS_H
#define TEPL_IO_ERROR_INFO_BARS_H

#if !defined (TEPL_H_INSIDE) && !defined (TEPL_COMPILATION)
#error "Only <tepl/tepl.h> can be included directly."
#endif

#include <gio/gio.h>
#include <tepl/tepl-info-bar.h>

G_BEGIN_DECLS

G_MODULE_EXPORT
TeplInfoBar *	tepl_io_error_info_bar_file_already_open		(GFile *location);

G_MODULE_EXPORT
TeplInfoBar *	tepl_io_error_info_bar_cant_create_backup		(GFile        *location,
									 const GError *error);

G_MODULE_EXPORT
TeplInfoBar *	tepl_io_error_info_bar_externally_modified		(GFile    *location,
									 gboolean  document_modified);

G_MODULE_EXPORT
TeplInfoBar *	tepl_io_error_info_bar_saving_externally_modified	(GFile *location);

G_MODULE_EXPORT
TeplInfoBar *	tepl_io_error_info_bar_invalid_characters		(GFile *location);

G_END_DECLS

#endif /* TEPL_IO_ERROR_INFO_BARS_H */
