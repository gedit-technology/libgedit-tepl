/* SPDX-FileCopyrightText: 2008 - Jesse van den Kieboom
 * SPDX-FileCopyrightText: 2023 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_STATUS_MENU_BUTTON_H
#define TEPL_STATUS_MENU_BUTTON_H

#if !defined (TEPL_H_INSIDE) && !defined (TEPL_COMPILATION)
#error "Only <tepl/tepl.h> can be included directly."
#endif

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define TEPL_TYPE_STATUS_MENU_BUTTON             (tepl_status_menu_button_get_type ())
#define TEPL_STATUS_MENU_BUTTON(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TEPL_TYPE_STATUS_MENU_BUTTON, TeplStatusMenuButton))
#define TEPL_STATUS_MENU_BUTTON_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TEPL_TYPE_STATUS_MENU_BUTTON, TeplStatusMenuButtonClass))
#define TEPL_IS_STATUS_MENU_BUTTON(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TEPL_TYPE_STATUS_MENU_BUTTON))
#define TEPL_IS_STATUS_MENU_BUTTON_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TEPL_TYPE_STATUS_MENU_BUTTON))
#define TEPL_STATUS_MENU_BUTTON_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TEPL_TYPE_STATUS_MENU_BUTTON, TeplStatusMenuButtonClass))

typedef struct _TeplStatusMenuButton         TeplStatusMenuButton;
typedef struct _TeplStatusMenuButtonClass    TeplStatusMenuButtonClass;
typedef struct _TeplStatusMenuButtonPrivate  TeplStatusMenuButtonPrivate;

struct _TeplStatusMenuButton
{
	GtkMenuButton parent;

	TeplStatusMenuButtonPrivate *priv;
};

struct _TeplStatusMenuButtonClass
{
	GtkMenuButtonClass parent_class;

	gpointer padding[12];
};

G_MODULE_EXPORT
GType			tepl_status_menu_button_get_type	(void);

G_MODULE_EXPORT
TeplStatusMenuButton *	tepl_status_menu_button_new		(void);

G_MODULE_EXPORT
void			tepl_status_menu_button_set_label_text	(TeplStatusMenuButton *button,
								 const gchar          *str);

G_END_DECLS

#endif /* TEPL_STATUS_MENU_BUTTON_H */
