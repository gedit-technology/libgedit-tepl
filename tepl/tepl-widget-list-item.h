/* SPDX-FileCopyrightText: 2022 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_WIDGET_LIST_ITEM_H
#define TEPL_WIDGET_LIST_ITEM_H

#if !defined (TEPL_H_INSIDE) && !defined (TEPL_COMPILATION)
#error "Only <tepl/tepl.h> can be included directly."
#endif

#include <gtk/gtk.h>
#include <tepl/tepl-macros.h>

G_BEGIN_DECLS

typedef struct _TeplWidgetListItem TeplWidgetListItem;

_TEPL_EXTERN
TeplWidgetListItem *	tepl_widget_list_item_new		(void);

_TEPL_EXTERN
void			tepl_widget_list_item_free		(TeplWidgetListItem *item);

_TEPL_EXTERN
void			tepl_widget_list_item_set_widget	(TeplWidgetListItem *item,
								 GtkWidget          *widget);

_TEPL_EXTERN
GtkWidget *		tepl_widget_list_item_get_widget	(const TeplWidgetListItem *item);

_TEPL_EXTERN
void			tepl_widget_list_item_set_displayed	(TeplWidgetListItem *item,
								 gboolean            displayed);

_TEPL_EXTERN
gboolean		tepl_widget_list_item_get_displayed	(const TeplWidgetListItem *item);

_TEPL_EXTERN
void			tepl_widget_list_item_set_infos		(TeplWidgetListItem *item,
								 const gchar        *name,
								 const gchar        *title,
								 const gchar        *icon_name);

_TEPL_EXTERN
void			tepl_widget_list_item_get_infos		(const TeplWidgetListItem  *item,
								 const gchar              **name,
								 const gchar              **title,
								 const gchar              **icon_name);

G_END_DECLS

#endif /* TEPL_WIDGET_LIST_ITEM_H */
