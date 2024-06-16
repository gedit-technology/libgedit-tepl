/* SPDX-FileCopyrightText: 2023-2024 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_PANEL_ITEM_H
#define TEPL_PANEL_ITEM_H

#if !defined (TEPL_H_INSIDE) && !defined (TEPL_COMPILATION)
#error "Only <tepl/tepl.h> can be included directly."
#endif

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define TEPL_TYPE_PANEL_ITEM             (tepl_panel_item_get_type ())
#define TEPL_PANEL_ITEM(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), TEPL_TYPE_PANEL_ITEM, TeplPanelItem))
#define TEPL_PANEL_ITEM_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), TEPL_TYPE_PANEL_ITEM, TeplPanelItemClass))
#define TEPL_IS_PANEL_ITEM(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TEPL_TYPE_PANEL_ITEM))
#define TEPL_IS_PANEL_ITEM_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), TEPL_TYPE_PANEL_ITEM))
#define TEPL_PANEL_ITEM_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), TEPL_TYPE_PANEL_ITEM, TeplPanelItemClass))

typedef struct _TeplPanelItem         TeplPanelItem;
typedef struct _TeplPanelItemClass    TeplPanelItemClass;
typedef struct _TeplPanelItemPrivate  TeplPanelItemPrivate;

struct _TeplPanelItem
{
	GObject parent;

	TeplPanelItemPrivate *priv;
};

struct _TeplPanelItemClass
{
	GObjectClass parent_class;

	gpointer padding[12];
};

G_MODULE_EXPORT
GType		tepl_panel_item_get_type		(void);

G_MODULE_EXPORT
TeplPanelItem *	tepl_panel_item_new			(GtkWidget   *widget,
							 const gchar *name,
							 const gchar *title,
							 const gchar *icon_name);

G_MODULE_EXPORT
GtkWidget *	tepl_panel_item_get_widget		(TeplPanelItem *item);

G_MODULE_EXPORT
const gchar *	tepl_panel_item_get_name		(TeplPanelItem *item);

G_MODULE_EXPORT
const gchar *	tepl_panel_item_get_title		(TeplPanelItem *item);

G_MODULE_EXPORT
const gchar *	tepl_panel_item_get_icon_name		(TeplPanelItem *item);

G_MODULE_EXPORT
gint		tepl_panel_item_get_position		(TeplPanelItem *item);

G_MODULE_EXPORT
gint		tepl_panel_item_compare			(TeplPanelItem *a,
							 TeplPanelItem *b);

G_MODULE_EXPORT
gint		tepl_panel_item_compare_by_title	(TeplPanelItem *a,
							 TeplPanelItem *b);

G_END_DECLS

#endif /* TEPL_PANEL_ITEM_H */
