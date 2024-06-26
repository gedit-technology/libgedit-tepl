/* SPDX-FileCopyrightText: 2017 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_APPLICATION_WINDOW_H
#define TEPL_APPLICATION_WINDOW_H

#if !defined (TEPL_H_INSIDE) && !defined (TEPL_COMPILATION)
#error "Only <tepl/tepl.h> can be included directly."
#endif

#include <gtk/gtk.h>
#include <tepl/tepl-tab-group.h>

G_BEGIN_DECLS

#define TEPL_TYPE_APPLICATION_WINDOW             (tepl_application_window_get_type ())
#define TEPL_APPLICATION_WINDOW(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TEPL_TYPE_APPLICATION_WINDOW, TeplApplicationWindow))
#define TEPL_APPLICATION_WINDOW_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TEPL_TYPE_APPLICATION_WINDOW, TeplApplicationWindowClass))
#define TEPL_IS_APPLICATION_WINDOW(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TEPL_TYPE_APPLICATION_WINDOW))
#define TEPL_IS_APPLICATION_WINDOW_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TEPL_TYPE_APPLICATION_WINDOW))
#define TEPL_APPLICATION_WINDOW_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TEPL_TYPE_APPLICATION_WINDOW, TeplApplicationWindowClass))

typedef struct _TeplApplicationWindow         TeplApplicationWindow;
typedef struct _TeplApplicationWindowClass    TeplApplicationWindowClass;
typedef struct _TeplApplicationWindowPrivate  TeplApplicationWindowPrivate;

struct _TeplApplicationWindow
{
	GObject parent;

	TeplApplicationWindowPrivate *priv;
};

struct _TeplApplicationWindowClass
{
	GObjectClass parent_class;

	gpointer padding[12];
};

G_MODULE_EXPORT
GType			tepl_application_window_get_type			(void) G_GNUC_CONST;

G_MODULE_EXPORT
TeplApplicationWindow *	tepl_application_window_get_from_gtk_application_window	(GtkApplicationWindow *gtk_window);

G_MODULE_EXPORT
GtkApplicationWindow *	tepl_application_window_get_application_window		(TeplApplicationWindow *tepl_window);

G_MODULE_EXPORT
void			tepl_application_window_set_tab_group			(TeplApplicationWindow *tepl_window,
										 TeplTabGroup          *tab_group);

G_MODULE_EXPORT
gboolean		tepl_application_window_is_main_window			(GtkApplicationWindow *gtk_window);

G_MODULE_EXPORT
GtkWindowGroup *	tepl_application_window_get_window_group		(TeplApplicationWindow *tepl_window);

G_MODULE_EXPORT
gboolean		tepl_application_window_get_handle_title		(TeplApplicationWindow *tepl_window);

G_MODULE_EXPORT
void			tepl_application_window_set_handle_title		(TeplApplicationWindow *tepl_window,
										 gboolean               handle_title);

G_MODULE_EXPORT
void			tepl_application_window_open_file			(TeplApplicationWindow *tepl_window,
										 GFile                 *location,
										 gboolean               jump_to);

G_END_DECLS

#endif /* TEPL_APPLICATION_WINDOW_H */
