/* SPDX-FileCopyrightText: 2016-2020 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_VIEW_H
#define TEPL_VIEW_H

#if !defined (TEPL_H_INSIDE) && !defined (TEPL_COMPILATION)
#error "Only <tepl/tepl.h> can be included directly."
#endif

#include <gtksourceview/gtksource.h>

G_BEGIN_DECLS

#define TEPL_TYPE_VIEW (tepl_view_get_type ())
G_MODULE_EXPORT
G_DECLARE_DERIVABLE_TYPE (TeplView, tepl_view,
			  TEPL, VIEW,
			  GtkSourceView)

struct _TeplViewClass
{
	GtkSourceViewClass parent_class;

	gpointer padding[12];
};

G_MODULE_EXPORT
GtkWidget *		tepl_view_new					(void);

G_MODULE_EXPORT
GtkWidget *		tepl_view_new_with_buffer			(GtkSourceBuffer *buffer);

G_MODULE_EXPORT
void			tepl_view_cut_clipboard				(TeplView *view);

G_MODULE_EXPORT
void			tepl_view_copy_clipboard			(TeplView *view);

G_MODULE_EXPORT
void			tepl_view_paste_clipboard			(TeplView *view);

G_MODULE_EXPORT
void			tepl_view_delete_selection			(TeplView *view);

G_MODULE_EXPORT
void			tepl_view_select_all				(TeplView *view);

G_MODULE_EXPORT
void			tepl_view_scroll_to_cursor			(TeplView *view);

G_MODULE_EXPORT
gboolean		tepl_view_goto_line				(TeplView *view,
									 gint      line);

G_MODULE_EXPORT
gboolean		tepl_view_goto_line_offset			(TeplView *view,
									 gint      line,
									 gint      line_offset);

G_MODULE_EXPORT
void			tepl_view_select_lines				(TeplView *view,
									 gint      start_line,
									 gint      end_line);

G_END_DECLS

#endif /* TEPL_VIEW_H */
