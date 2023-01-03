/* SPDX-FileCopyrightText: 2022-2023 - Sébastien Wilmet <swilmet@gnome.org>
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

#define TEPL_TYPE_WIDGET_LIST_ITEM             (tepl_widget_list_item_get_type ())
#define TEPL_WIDGET_LIST_ITEM(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TEPL_TYPE_WIDGET_LIST_ITEM, TeplWidgetListItem))
#define TEPL_WIDGET_LIST_ITEM_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TEPL_TYPE_WIDGET_LIST_ITEM, TeplWidgetListItemClass))
#define TEPL_IS_WIDGET_LIST_ITEM(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TEPL_TYPE_WIDGET_LIST_ITEM))
#define TEPL_IS_WIDGET_LIST_ITEM_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TEPL_TYPE_WIDGET_LIST_ITEM))
#define TEPL_WIDGET_LIST_ITEM_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TEPL_TYPE_WIDGET_LIST_ITEM, TeplWidgetListItemClass))

typedef struct _TeplWidgetListItem         TeplWidgetListItem;
typedef struct _TeplWidgetListItemClass    TeplWidgetListItemClass;
typedef struct _TeplWidgetListItemPrivate  TeplWidgetListItemPrivate;

struct _TeplWidgetListItem
{
	GObject parent;

	TeplWidgetListItemPrivate *priv;
};

struct _TeplWidgetListItemClass
{
	GObjectClass parent_class;

	gpointer padding[12];
};

_TEPL_EXTERN
GType			tepl_widget_list_item_get_type		(void);

_TEPL_EXTERN
TeplWidgetListItem *	tepl_widget_list_item_new		(void);

_TEPL_EXTERN
void			tepl_widget_list_item_set_widget	(TeplWidgetListItem *item,
								 GtkWidget          *widget);

_TEPL_EXTERN
GtkWidget *		tepl_widget_list_item_get_widget	(TeplWidgetListItem *item);

_TEPL_EXTERN
void			tepl_widget_list_item_set_displayed	(TeplWidgetListItem *item,
								 gboolean            displayed);

_TEPL_EXTERN
gboolean		tepl_widget_list_item_get_displayed	(TeplWidgetListItem *item);

_TEPL_EXTERN
void			tepl_widget_list_item_set_infos		(TeplWidgetListItem *item,
								 const gchar        *name,
								 const gchar        *title,
								 const gchar        *icon_name);

_TEPL_EXTERN
void			tepl_widget_list_item_get_infos		(TeplWidgetListItem  *item,
								 gchar              **name,
								 gchar              **title,
								 gchar              **icon_name);

G_END_DECLS

#endif /* TEPL_WIDGET_LIST_ITEM_H */
