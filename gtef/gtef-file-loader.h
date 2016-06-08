/*
 * This file is part of Gtef, a text editor library.
 *
 * Copyright 2016 - Sébastien Wilmet <swilmet@gnome.org>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GTEF_FILE_LOADER_H
#define GTEF_FILE_LOADER_H

#if !defined (GTEF_H_INSIDE) && !defined (GTEF_COMPILATION)
#error "Only <gtef/gtef.h> can be included directly."
#endif

#include <gio/gio.h>
#include <gtef/gtef-types.h>

G_BEGIN_DECLS

#define GTEF_TYPE_FILE_LOADER (gtef_file_loader_get_type ())
G_DECLARE_DERIVABLE_TYPE (GtefFileLoader, gtef_file_loader,
			  GTEF, FILE_LOADER,
			  GObject)

struct _GtefFileLoaderClass
{
	GObjectClass parent_class;

	gpointer padding[12];
};

GtefFileLoader *	gtef_file_loader_new					(GtefBuffer *buffer);

GtefBuffer *		gtef_file_loader_get_buffer				(GtefFileLoader *loader);

GFile *			gtef_file_loader_get_location				(GtefFileLoader *loader);

void			gtef_file_loader_load_async				(GtefFileLoader      *loader,
										 gint                 io_priority,
										 GCancellable        *cancellable,
										 GAsyncReadyCallback  callback,
										 gpointer             user_data);

gboolean		gtef_file_loader_load_finish				(GtefFileLoader  *loader,
										 GAsyncResult    *result,
										 GError         **error);

G_END_DECLS

#endif /* GTEF_FILE_LOADER_H */
