/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet <swilmet@gnome.org>
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef TEPL_PANEL_ITEM_H
#define TEPL_PANEL_ITEM_H

#if !defined (TEPL_H_INSIDE) && !defined (TEPL_COMPILATION)
#error "Only <tepl/tepl.h> can be included directly."
#endif

#include <gtk/gtk.h>
#include <tepl/tepl-macros.h>

G_BEGIN_DECLS

#define TEPL_TYPE_PANEL_ITEM               (tepl_panel_item_get_type ())
#define TEPL_PANEL_ITEM(obj)               (G_TYPE_CHECK_INSTANCE_CAST ((obj), TEPL_TYPE_PANEL_ITEM, TeplPanelItem))
#define TEPL_IS_PANEL_ITEM(obj)            (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TEPL_TYPE_PANEL_ITEM))
#define TEPL_PANEL_ITEM_GET_INTERFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE ((obj), TEPL_TYPE_PANEL_ITEM, TeplPanelItemInterface))

typedef struct _TeplPanelItem          TeplPanelItem;
typedef struct _TeplPanelItemInterface TeplPanelItemInterface;

/**
 * TeplPanelItemInterface:
 * @parent_interface: The parent interface.
 * @get_widget: The virtual function pointer for tepl_panel_item_get_widget().
 *   By default, returns %NULL.
 * @get_name: The virtual function pointer for tepl_panel_item_get_name().
 *   By default, returns %NULL.
 * @get_title: The virtual function pointer for tepl_panel_item_get_title().
 *   By default, returns %NULL.
 * @get_icon_name: The virtual function pointer for tepl_panel_item_get_icon_name().
 *   By default, returns %NULL.
 *
 * The virtual function table for #TeplPanelItem.
 *
 * Since: 6.8
 */
struct _TeplPanelItemInterface
{
	GTypeInterface parent_interface;

	GtkWidget *	(*get_widget)		(TeplPanelItem *item);

	const gchar *	(*get_name)		(TeplPanelItem *item);

	const gchar *	(*get_title)		(TeplPanelItem *item);

	const gchar *	(*get_icon_name)	(TeplPanelItem *item);
};

_TEPL_EXTERN
GType		tepl_panel_item_get_type		(void);

_TEPL_EXTERN
GtkWidget *	tepl_panel_item_get_widget		(TeplPanelItem *item);

_TEPL_EXTERN
const gchar *	tepl_panel_item_get_name		(TeplPanelItem *item);

_TEPL_EXTERN
const gchar *	tepl_panel_item_get_title		(TeplPanelItem *item);

_TEPL_EXTERN
const gchar *	tepl_panel_item_get_icon_name		(TeplPanelItem *item);

_TEPL_EXTERN
gint		tepl_panel_item_compare_by_title	(TeplPanelItem *a,
							 TeplPanelItem *b);

G_END_DECLS

#endif /* TEPL_PANEL_ITEM_H */
